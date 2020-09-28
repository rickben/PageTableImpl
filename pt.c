#include <stdio.h>
#include "os.h"

uint64_t index_for_curr_level(uint64_t vpn, int level){
    return ((vpn << (9*(level)-2) ) >> 55);
    /* get the bits: 56-48*/
    /* get the bits: 47-39*/
    /* get the bits: 38-30*/
    /* get the bits: 29-21*/
    /* get the bits: 12-20*/
}

void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn){
    uint64_t level_index = 0 , pt_address = pt << 12 , new_address = 0, valid_bit = 0;
    uint64_t* curr_level_frame;
    int i;
    for (i=1; i<6; i++){
        curr_level_frame = (uint64_t*) phys_to_virt(pt_address);
        level_index = index_for_curr_level(vpn,i);
        valid_bit = curr_level_frame[level_index] & 1;
        if (i < 5){
           if (valid_bit == 0){ /* No reference address */
               if (ppn != NO_MAPPING){
                    new_address = alloc_page_frame();
                    curr_level_frame[level_index] = (new_address << 12) | 1;
                    pt_address = new_address << 12;
               }else{
                    break;
            }
           }else{ /* Already Mapped to next frame */
               pt_address = curr_level_frame[level_index] - 1; /* Remove the valid bit from the offset */
           }
        } else {
                if (ppn != NO_MAPPING)
                    curr_level_frame[level_index] = (ppn << 12) | 1;
                else
                    curr_level_frame[level_index] = 0;
	}
  }
}

uint64_t page_table_query(uint64_t pt, uint64_t vpn){
    uint64_t level_index = 0 , pt_address = pt << 12 , valid_bit = 0;
    uint64_t* curr_level_frame;
    int i;
    for (i=1; i<6; i++){
        curr_level_frame = (uint64_t*) phys_to_virt(pt_address);
        level_index = index_for_curr_level(vpn,i);
        valid_bit = curr_level_frame[level_index] & 1;
        if (valid_bit == 0) { /* No reference address */
            return NO_MAPPING;
        } else {
                pt_address = curr_level_frame[level_index] - 1; /* Remove the valid bit from the offset */
        }
    }
    return pt_address >> 12;
}

