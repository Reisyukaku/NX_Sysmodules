#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <dirent.h>

#include <switch.h>
#include "ldr_patcher.hpp"

/* Patching adapted from ReiNand (https://github.com/Reisyukaku/loader/blob/4dfda7797df965177d8d7060a21852261f7310a5/source/patcher.c) */

// Below is stolen from http://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string_search_algorithm

#define ALPHABET_LEN 256
#define NOT_FOUND patlen
#define max(a, b) ((a < b) ? b : a)

// delta1 table: delta1[c] contains the distance between the last
// character of pat and the rightmost occurence of c in pat.
// If c does not occur in pat, then delta1[c] = patlen.
// If c is at string[i] and c != pat[patlen-1], we can
// safely shift i over by delta1[c], which is the minimum distance
// needed to shift pat forward to get string[i] lined up 
// with some character in pat.
// this algorithm runs in alphabet_len+patlen time.
static void make_delta1(int *delta1, u8 *pat, int patlen){
    int i;
    for (i=0; i < ALPHABET_LEN; i++) delta1[i] = NOT_FOUND;
    for (i=0; i < patlen-1; i++) delta1[pat[i]] = patlen-1 - i;
}
 
// true if the suffix of word starting from word[pos] is a prefix 
// of word
static int is_prefix(u8 *word, int wordlen, int pos){
    int i, suffixlen = wordlen - pos;
    for (i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos+i]) return 0;
    }
    return 1;
}

// length of the longest suffix of word ending on word[pos].
// suffix_length("dddbcabc", 8, 4) = 2
static int suffix_length(u8 *word, int wordlen, int pos){
    int i;
    // increment suffix length i to the first mismatch or beginning
    // of the word
    for (i = 0; (word[pos-i] == word[wordlen-1-i]) && (i < pos); i++);
    return i;
}

// delta2 table: given a mismatch at pat[pos], we want to align 
// with the next possible full match could be based on what we
// know about pat[pos+1] to pat[patlen-1].
//
// In case 1:
// pat[pos+1] to pat[patlen-1] does not occur elsewhere in pat,
// the next plausible match starts at or after the mismatch.
// If, within the substring pat[pos+1 .. patlen-1], lies a prefix
// of pat, the next plausible match is here (if there are multiple
// prefixes in the substring, pick the longest). Otherwise, the
// next plausible match starts past the character aligned with 
// pat[patlen-1].
// 
// In case 2:
// pat[pos+1] to pat[patlen-1] does occur elsewhere in pat. The
// mismatch tells us that we are not looking at the end of a match.
// We may, however, be looking at the middle of a match.
// 
// The first loop, which takes care of case 1, is analogous to
// the KMP table, adapted for a 'backwards' scan order with the
// additional restriction that the substrings it considers as 
// potential prefixes are all suffixes. In the worst case scenario
// pat consists of the same letter repeated, so every suffix is
// a prefix. This loop alone is not sufficient, however:
// Suppose that pat is "ABYXCDEYX", and text is ".....ABYXCDEYX".
// We will match X, Y, and find B != E. There is no prefix of pat
// in the suffix "YX", so the first loop tells us to skip forward
// by 9 characters.
// Although superficially similar to the KMP table, the KMP table
// relies on information about the beginning of the partial match
// that the BM algorithm does not have.
//
// The second loop addresses case 2. Since suffix_length may not be
// unique, we want to take the minimum value, which will tell us
// how far away the closest potential match is.
static void make_delta2(int *delta2, u8 *pat, int patlen){
    int p;
    int last_prefix_index = patlen-1;
  
    // first loop
    for (p=patlen-1; p>=0; p--) {
        if (is_prefix(pat, patlen, p+1)) {
            last_prefix_index = p+1;
        }
        delta2[p] = last_prefix_index + (patlen-1 - p);
    }
 
    // second loop
    for (p=0; p < patlen-1; p++) {
        int slen = suffix_length(pat, patlen, p);
        if (pat[p - slen] != pat[patlen-1 - slen]) {
            delta2[patlen-1 - slen] = patlen-1 - p + slen;
        }
    }
}
 
static u8* boyer_moore(u8 *string, int stringlen, u8 *pat, int patlen){
    int i;
    int delta1[ALPHABET_LEN];
    int delta2[patlen * sizeof(int)];
    make_delta1(delta1, pat, patlen);
    make_delta2(delta2, pat, patlen);
 
  i = patlen-1;
    while (i < stringlen) {
        int j = patlen-1;
        while (j >= 0 && (string[i] == pat[j])) {
            --i;
            --j;
        }
        if (j < 0) return (string + i+1);
        i += max(delta1[string[i]], delta2[j]);
    }
    return NULL;
}

static int patch_memory(u8 *start, u32 size, u8 *pattern, u32 patsize, int offset, u8 *replace, u32 repsize, int count)
{
    u8 *found;
    int i;
    u32 at;

    for (i = 0; i < count; i++){
        found = boyer_moore(start, size, pattern, patsize);
        if (found == NULL) break;
        at = (u32)(found - start);
        memcpy(found + offset, replace, repsize);
        if (at + patsize > size) size = 0;
        else size = size - (at + patsize);
        start = found + patsize;
    }
    return i;
}

static void apply_patch(FILE* patch_file, u8 *mapped_nso, size_t mapped_size)
{
    u8 patch_count;
    u8 pattern_length;
    u8 patch_length;
    s8 search_multiple;
    s8 offset;
    u8 pattern[0x100] = {0};
    u8 patch[0x100] = {0};

    if (fread(&patch_count, 1, 1, patch_file) != 1) return;

    for (int i = 0; i < patch_count; i++)
    {
        if (fread(&pattern_length, 1, 1, patch_file) != 1) return;
        if (fread(&patch_length, 1, 1, patch_file) != 1) return;
        if (fread(&search_multiple, 1, 1, patch_file) != 1) return;
        if (fread(&offset, 1, 1, patch_file) != 1) return;
        if (fread(pattern, pattern_length, 1, patch_file) != 1) return;
        if (fread(patch, patch_length, 1, patch_file) != 1) return;

        patch_memory(mapped_nso, mapped_size, pattern, pattern_length, offset, patch, patch_length, search_multiple);
    }
}

void Patcher::ApplyPatches(u64 title_id, u8 *mapped_nso, size_t mapped_size) {
    char magic[4] = {0};
    u64 read_id;

    /* Inspect all patches from /ReiNX/patches/<*> */
    char path[FS_MAX_PATH+1] = {0};
    snprintf(path, sizeof(path) - 1, "sdmc:/ReiNX/patches");
    DIR *patches_dir = opendir(path);
    struct dirent *ent;
    if (patches_dir != NULL) {
        while ((ent = readdir(patches_dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            snprintf(path, sizeof(path) - 1, "sdmc:/ReiNX/patches/%s", ent->d_name);
            FILE *patch_file = fopen(path, "rb");
            if (patch_file != NULL)
            {
                fread(magic, 3, 1, patch_file);
                fread(&read_id, 8, 1, patch_file);
                if (strcmp(magic, "RXP") == 0 && read_id == title_id)
                {
                    apply_patch(patch_file, mapped_nso, mapped_size);
                }

                fclose(patch_file);
            }
        }

        closedir(patches_dir);
    }
}