#include "common.h"
#include "utils.h"
#include "path_utils.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <image file name> <path>\n", argv[0]);
        exit(1);
    }
    init_disk(argv[1]); 

    char *path = argv[2];
    if (path[0] != '/'){
        perror('Not absolute path');
        return ENOENT;
    }

    PathData_t *pd = split_path(path, NULL);
    int parent_inode_num = get_parent_inode(pd);

    if (new_file_exists(parent_inode_num, pd, EXT2_FT_REG_FILE) || new_file_exists(parent_inode_num, pd, EXT2_FT_SYMLINK)) {
        return EEXIST;
    }

    struct ext2_inode *parent_inode = get_inode_with_num((unsigned int) parent_inode_num);
    struct ext2_dir_entry *containing_dir_ent = (struct ext2_dir_entry *) (disk + EXT2_BLOCK_SIZE * parent_inode->i_block[0]);

    struct ext2_super_block *sb = get_super_block();
    struct ext2_inode *inode_table = get_inode_table();


    for (int i = 0; i < sb->s_inodes_count; i++) {
        if (is_valid(get_inode_map(), i) && (i == parent_inode_num -1)) {
            struct ext2_inode curr_inode = inode_table[i];
            // following line will take us to the parent dir entry. (we want to look at all dir entries WITHIN this guy)
            struct ext2_dir_entry *curr_dir = (struct ext2_dir_entry *) (disk + EXT2_BLOCK_SIZE * curr_inode.i_block[0]);

            int traversed_len = 0;
            int rec_len = 0;

            while (traversed_len  < EXT2_BLOCK_SIZE) {
                curr_dir = (void *) curr_dir + rec_len;
                // We are now within a directory in side the PARENT of our deleted file.
                // We must check its padding to see if our deleted entry lies there
                int total_rec_len = get_rec_len(curr_dir->name);
                int padding = curr_dir->rec_len - get_rec_len(curr_dir->name);

                if (padding > 0)  {
                    struct ext2_dir_entry *deleted_dir_entry = (void *) curr_dir + get_rec_len(curr_dir->name);

                    if (deleted_dir_entry->inode > 0) {
                        if (!is_valid(get_inode_map(), deleted_dir_entry->inode)) {
                            if (strlen(pd->file_name) == deleted_dir_entry->name_len && strncmp(pd->file_name, deleted_dir_entry->name, deleted_dir_entry->name_len) == 0) {
                                for (int j = 0; j < 15; j++) {
                                    //index

                                    int free_block = allocate_next_free(BLOCK);
                                    int free_inode = allocate_next_free(INODE);
                                    if (free_block == -1 || free_inode == -1){
                                        perror("No space");
                                        return ENOSPC;
                                    }

                                    // initialize new inode
                                    init_inode(deleted_dir_entry->file_type, free_inode, free_block);


                                }
                            }
                        }
                        // check inode bitmap.
                            // used? Cant do anything
                            // else: find name & name_len matching the one were looking for
                            /**
                             * name and name_len matches,
                             *      restore by reallocating inode in bitmap, increasing link count
                             *      set d time to 0, fix the paddings of prev ones to point to newly
                             *      restored inode.
                             *
                             *      Also set rec_len of prev entry to point to the start of this new
                             *      restored file
                             */
                        struct ext2_inode *inode_bitmap
                    }
                }

            }

        }
    }

    while (traversed_len < EXT2_BLOCK_SIZE) {
        curr_dir = (void *) curr_dir + rec_len;
    }
}