/*
 * Copyright (c) 2018 Atmosphère-NX, ReiSwitched
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <ctype.h>

#include <switch.h>
#include "ldr_patcher.hpp"

/* IPS Patching adapted from Luma3DS (https://github.com/AuroraWright/Luma3DS/blob/master/sysmodules/loader/source/patcher.c) */

#define IPS_MAGIC "PATCH"
#define IPS_TAIL "EOF"

#define IPS32_MAGIC "IPS32"
#define IPS32_TAIL "EEOF"

static inline u8 HexNybbleToU8(const char nybble) {
    if ('0' <= nybble && nybble <= '9') {
        return nybble - '0';
    } else if ('a' <= nybble && nybble <= 'f') {
        return nybble - 'a' + 0xa;
    } else {
        return nybble - 'A' + 0xA;
    }
}

static bool MatchesBuildId(const char *name, size_t name_len, const u8 *build_id) {
    /* Validate name is hex build id. */
    for (unsigned int i = 0; i < name_len - 4; i++) {
        if (isxdigit(name[i]) == 0) {
                return false;
        }
    }
    
    /* Read build id from name. */
    u8 build_id_from_name[0x20] = {0};
    for (unsigned int name_ofs = 0, id_ofs = 0; name_ofs < name_len - 4; id_ofs++) {
        build_id_from_name[id_ofs] |= HexNybbleToU8(name[name_ofs++]) << 4;
        build_id_from_name[id_ofs] |= HexNybbleToU8(name[name_ofs++]);
    }
    
    return memcmp(build_id, build_id_from_name, sizeof(build_id_from_name)) == 0;
}

static void ApplyIpsPatch(u8 *mapped_nso, size_t mapped_size, bool is_ips32, FILE *f_ips) {
    u8 buffer[4];
    while (fread(buffer, is_ips32 ? 4 : 3, 1, f_ips) == 1) {
        if (is_ips32 && memcmp(buffer, IPS32_TAIL, 4) == 0) {
            break;
        } else if (!is_ips32 && memcmp(buffer, IPS_TAIL, 3) == 0) {
            break;
        }
        
        /* Offset of patch. */
        u32 patch_offset;
        if (is_ips32) {
           patch_offset = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
        } else {
           patch_offset = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);
        }
        
        /* Size of patch. */
        if (fread(buffer, 2, 1, f_ips) != 1) {
            break;
        }
        u32 patch_size = (buffer[0] << 8) | (buffer[1]);
        
        /* Check for RLE encoding. */
        if (patch_size == 0) {
            /* Size of RLE. */
            if (fread(buffer, 2, 1, f_ips) != 1) {
                break;
            }
            
            u32 rle_size = (buffer[0] << 8) | (buffer[1]);
            
            /* Value for RLE. */
            if (fread(buffer, 1, 1, f_ips) != 1) {
                break;
            }
            
            if (patch_offset < sizeof(NsoUtils::NsoHeader)) {
                if (patch_offset + rle_size > sizeof(NsoUtils::NsoHeader)) {
                    u32 diff = sizeof(NsoUtils::NsoHeader) - patch_offset;
                    patch_offset += diff;
                    rle_size -= diff;
                    goto IPS_RLE_PATCH_OFFSET_WITHIN_BOUNDS;
                }
            } else {
                IPS_RLE_PATCH_OFFSET_WITHIN_BOUNDS:
                patch_offset -= sizeof(NsoUtils::NsoHeader);
                if (patch_offset + rle_size > mapped_size) {
                    rle_size = mapped_size - patch_offset;
                }
                memset(mapped_nso + patch_offset, buffer[0], rle_size);
            }
        } else {
            if (patch_offset < sizeof(NsoUtils::NsoHeader)) {
                if (patch_offset + patch_size > sizeof(NsoUtils::NsoHeader)) {
                    u32 diff = sizeof(NsoUtils::NsoHeader) - patch_offset;
                    patch_offset += diff;
                    patch_size -= diff;
                    fseek(f_ips, diff, SEEK_CUR);
                    goto IPS_DATA_PATCH_OFFSET_WITHIN_BOUNDS;
                } else {
                    fseek(f_ips, patch_size, SEEK_CUR);
                }
            } else {
                IPS_DATA_PATCH_OFFSET_WITHIN_BOUNDS:
                patch_offset -= sizeof(NsoUtils::NsoHeader);
                u32 read_size = patch_size;
                if (patch_offset + read_size > mapped_size) {
                    read_size = mapped_size - patch_offset;
                }
                if (fread(mapped_nso + patch_offset, read_size, 1, f_ips) != 1) {
                    break;
                }
                if (patch_size > read_size) {
                    fseek(f_ips, patch_size - read_size, SEEK_CUR);
                }
            }
        }
    }
}

static int is_prefix(u8 *word, int wordlen, int pos){
    int i, suffixlen = wordlen - pos;
    for (i = 0; i < suffixlen; i++) {
        if (word[i] != word[pos+i]) return 0;
    }
    return 1;
}

static u8* boyer_moore(u8 *string, int stringlen, u8 *pat, int patlen){
    int delta1[256];
    int delta2[patlen * sizeof(int)];
    int i, p;
    for (i=0; i < 256; i++) delta1[i] = patlen;
    for (i=0; i < patlen-1; i++) delta1[pat[i]] = patlen-1 - i;
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
        for (i = 0; (pat[p-i] == pat[patlen-1-i]) && (i < p); i++);
        int slen = i;
        if (pat[p - slen] != pat[patlen-1 - slen]) {
            delta2[patlen-1 - slen] = patlen-1 - p + slen;
        }
    }
 
    i = patlen-1;
    while (i < stringlen) {
        int j = patlen-1;
        while (j >= 0 && (string[i] == pat[j])) {
            --i;
            --j;
        }
        if (j < 0) return (string + i+1);
        i += ((delta1[string[i]] < delta2[j]) ? delta2[j] : delta1[string[i]]);
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

static void ApplyRnxPatch(FILE* patch_file, u8 *mapped_nso, size_t mapped_size)
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

void PatchUtils::ApplyPatches(const NsoUtils::NsoHeader *header, u64 title_id, u8 *mapped_nso, size_t mapped_size) {
    /* Inspect all patches from /ReiNX/patches/<*>/<*>.ips */
    char magic[4] = {0};
    u64 read_id;
    char path[FS_MAX_PATH+1] = {0};
    snprintf(path, sizeof(path) - 1, "sdmc:/ReiNX/patches");
    DIR *patches_dir = opendir(path);
    struct dirent *pdir_ent;
    if (patches_dir != NULL) {
        /* Iterate over the patches directory to find patch subdirectories. */
        while ((pdir_ent = readdir(patches_dir)) != NULL) {
            if (strcmp(pdir_ent->d_name, ".") == 0 || strcmp(pdir_ent->d_name, "..") == 0) {
                continue;
            }
            snprintf(path, sizeof(path) - 1, "sdmc:/ReiNX/patches/%s", pdir_ent->d_name);
            DIR *patch_dir = opendir(path);
            struct dirent *ent;
            if (patch_dir != NULL) {
                /* Iterate over the patch subdirectory to find .ips patches. */
                while ((ent = readdir(patch_dir)) != NULL) {
                    if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                        continue;
                    }
                    
                    size_t name_len = strlen(ent->d_name);
                    if ((4 < name_len && name_len <= 0x44) && ((name_len & 1) == 0) && strcmp(ent->d_name + name_len - 4, ".ips") == 0 && MatchesBuildId(ent->d_name, name_len, header->build_id)) {
                        snprintf(path, sizeof(path) - 1, "sdmc:/ReiNX/patches/%s/%s", pdir_ent->d_name, ent->d_name);
                        FILE *f_ips = fopen(path, "rb");
                        if (f_ips != NULL) {
                            u8 header[5];
                            if (fread(header, 5, 1, f_ips) == 1) {
                                if (memcmp(header, IPS_MAGIC, 5) == 0) {
                                    ApplyIpsPatch(mapped_nso, mapped_size, false, f_ips);
                                } else if (memcmp(header, IPS32_MAGIC, 5) == 0) {
                                    ApplyIpsPatch(mapped_nso, mapped_size, true, f_ips);
                                }
                            }
                            fclose(f_ips);
                        }
                    }
                }
                closedir(patch_dir);
            } else {
                FILE *patch_file = fopen(path, "rb");
                if (patch_file != NULL)
                {
                    fread(magic, 3, 1, patch_file);
                    fread(&read_id, 8, 1, patch_file);
                    if (strcmp(magic, "RXP") == 0 && read_id == title_id)
                    {
                        ApplyRnxPatch(patch_file, mapped_nso, mapped_size);
                    }

                    fclose(patch_file);
                }
            }
        }
        closedir(patches_dir);
    }
}