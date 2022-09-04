#include <a_samp>
#include <YSI\y_hooks>
#include <YSI\y_commands>

#include <foodstands>

enum {

    dialog_hotdog_stand,
    dialog_ice_cream_stand,
    dialog_noodle_stand
}

main ( ){

    print("Food Stands by Nodi - LOADED");
}

public OnGameModeInit( ){

    //Creating a food stand
    CreateFoodStand(15.00, 12.00, 13.00, 1, 0);
    return 1;
}

YCMD:buyfood(playerid, params[ ], help){

    #pragma unused help
    new stand = GetFoodStandNearPlayer(playerid);
    if(IsPlayerNearFoodStand(playerid) == false ) return SendClientMessage(playerid, -1, "You are not near a food stand!");
    if(fStandInfo[stand][fsType] == 1) {
        ShowPlayerDialog(playerid, dialog_hotdog_stand, DIALOG_STYLE_LIST, "Hotdog - Stand", "[1] >> Hotdog - Classic\n[2] >> Corn Dog\n[3] >>Bratwurst Hotdog", "Buy", "Leave");
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){

    if(dialogid == dialog_hotdog_stand){

        if(!response) return 0;
        if(response){
            switch(listitem){
                case 0:{
                    SendClientMessage(playerid, -1, "Selected - Hotdog Classic!");
                }
                case 1:{
                    SendClientMessage(playerid, -1, "Selected - Corn Dog!");
                }
                case 2:{
                    SendClientMessage(playerid, -1, "Selected - Bratwurst Hotdog!");
                }
            }
        }
    }
    return 1;
}