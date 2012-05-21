/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/input.h>
#include <sys/stat.h>

#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"
#include "libcrecovery/common.h"

char* MENU_HEADERS[] = { NULL };

char* MENU_ITEMS[] = { "reboot system now",
                       "install zip from sdcard",
                       "wipe data/factory reset",
                       "wipe cache partition",
                       "backup and restore",
                       "mounts and storage",
                       "cleanup tab",
                       "advanced",
                       NULL, /* padding if we add an item */
                       NULL};

/*If bootloader has multiboot support and the recovery contains
 setbootmode, this is inserted at item 1 above */
char* SET_BOOTMODE = "set boot mode";
int IS_MULTIBOOT;

void device_ui_init(UIParameters* ui_parameters) {
}

int device_recovery_start(){
    char tmp[60];
    int idx;
    char* hold=(char*)NULL;
    char* hold2=(char*)NULL;
    struct stat st;
    
    printf("Check if setbootmode exists\n");
    if(stat("/sbin/setbootmode",&st) == 0){
        printf("setbootmode found, check bootloader\n");
        /*we have the setbootmode binary*/
        /*check known bootloaders w/o multiboot support*/
        FILE *fp = __popen("awk '{m=match($0,/bootloader_ver=([0-9a-zA-Z\\.\\-]*)/) ; if(m){print(substr($0,RSTART+15,RLENGTH-15))}else{print(\"HC3.0.1\")}}' < /proc/cmdline","r");
        if(fp == NULL){
            ui_print("WARN: unable to parse bootloader from kernel commandline\n");
            IS_MULTIBOOT=0; /*error no multiboot*/
            return 0;
        }
        fgets(tmp,60,fp);
        __pclose(fp);
        tmp[59]=0;
        if(tmp[strlen(tmp)-1] == '\n')
            tmp[strlen(tmp)-1]=0;
        ui_print("Bootloader: %s\n",tmp);
        if(strcmp(tmp,"0.03.11-ICS")==0 || 
           strcmp(tmp,"0.03.12-UNL")==0 ||
           strcmp(tmp,"0.03.14-UNL")==0)
        {
            printf("bootloader missing multi-boot support\n"); 
            IS_MULTIBOOT=0;
            return 0;
        }
        printf("add set boot mode menu item to main menu\n");
        /*Prepare multiboot (edit menu)*/
        hold=MENU_ITEMS[2];
        MENU_ITEMS[2]=MENU_ITEMS[1];
        MENU_ITEMS[1]=SET_BOOTMODE;
        idx=3;
        while(hold != NULL){
            hold2=hold;
            hold=MENU_ITEMS[idx];
            MENU_ITEMS[idx]=hold2;
            idx++;
        }
        IS_MULTIBOOT=1;
    } else {
        printf("no setbootmode, multiboot disabled\n");
        /*If we don't have the setbootmode binary we can't support multiboot*/
        IS_MULTIBOOT=0;
    }
    return 0;
}

void multiboot_menu(){
    char* headers[] = {
        "Set Boot Mode!",
        NULL, /*will be replaced by the current boot status */
        NULL
    };
    char* list[] = {
        "cancel",
        "set primary boot",
        "set secondary boot",
        NULL
    };
    char tmp[80];
    FILE *fp;
    int chosen_item;

    /*start by reading in the bootmode state */
    fp = __popen("setbootmode --status","r");
    if(fp == NULL){
        ui_print("ERROR: cant read current bootmode\n");
        return;
    }
    fgets(tmp,80,fp);
    __pclose(fp);
    tmp[79]=0;
    if(tmp[strlen(tmp)-1] == '\n')
        tmp[strlen(tmp)-1]=0;
    headers[1]=tmp;

    /*now display the menu*/
    chosen_item = get_menu_selection(headers, list, 0, 0);
    switch(chosen_item){
        case 1:
            __system("setbootmode b1");
            ui_print("Primary bootmode set\n");
            break;
        case 2:
            __system("setbootmode b2");
            ui_print("Secondary bootmode set\n");
            break;
    }
}

int device_reboot_now(volatile char* key_pressed, int key_code) {
    return 0;
}

int device_perform_action(int which) {
    if(IS_MULTIBOOT && which >= 1)
    {
        if(which == 1){
            multiboot_menu();
            return NO_ACTION; /*nop*/
        }
        which--; /* subtract one and continue */
    }
    if(which == 6){
        system_verification_and_cleanup();
        return NO_ACTION; /*nop*/
    }
    if(which == 7){
        return ITEM_ADVANCED;  /*advanced*/
    }
    return which;
}

int device_wipe_data() {
    return 0;
}
