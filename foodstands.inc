/*

  - Name : Dynamic Food Stands
  - Date : 5.8.2022
  - Github : DinoWETT
  - Credits : //
  - Note : Morate imate kreiran folder "FoodStands" u scriptfles folderu!

*/
  

#if defined _foodstands_INCLUDED
    #endinput
#endif
#define _foodstands_INCLUDED
#include <a_samp>
#tryinclude <YSI\y_hooks>
#tryinclude <YSI\y_va>
#tryinclude <YSI\y_ini>
#tryinclude <streamer>

//              >> [ DEFINES ] <<

#if defined FSTAND_FILE
    #undef FSTAND_FILE
#else
    #define FSTAND_FILE "FoodStands/Stand_%d.ini"
#endif
#if defined MAX_FOOD_STANDS
    #undef MAX_FOOD_STANDS
#else
    #define MAX_FOOD_STANDS 30
#endif

#define INVALID_FOOD_STAND_ID -1

//              >> [ VARIABLES ] <<

static 
    bool:__g_PlayerIsEating[MAX_PLAYERS],
    __g_fscounter;

//              >> [ ENUMS ] <<

enum e_FOOD_STAND_DATA {

    fsID,
    Float:fstand_pos[3]
}

new fStandInfo[MAX_FOOD_STANDS][e_FOOD_STAND_DATA],
    Text3D:fStandLabel[MAX_FOOD_STANDS];

enum {
    dialog_FoodStand
}

forward LoadFoodStandData( fsid, name[ ], value[ ]);
public LoadFoodStandData( fsid, name[ ], value[ ]){

    INI_Int("ID", fStandInfo[fsid][fsID]);
    INI_Float("Pos_X", fStandInfo[fsid][fstand_pos][0]);
    INI_Float("Pos_Y", fStandInfo[fsid][fstand_pos][1]);
    INI_Float("Pos_Z", fStandInfo[fsid][fstand_pos][2]);
    return 1;
}

stock SaveFoodStand( fsid ){

    new fsFile[ 40 ];
    format(fsFile, sizeof(fsFile), FSTAND_FILE, fsid);
    new INI:File = INI_Open(fsFile);

    INI_WriteInt(File, "ID", fStandInfo[fsid][fsID]);
    INI_WriteFloat(File, "Pos_X", fStandInfo[fsid][fstand_pos][0]);
    INI_WriteFloat(File, "Pos_Y", fStandInfo[fsid][fstand_pos][1]);
    INI_WriteFloat(File, "Pos_Z", fStandInfo[fsid][fstand_pos][2]);
    INI_Close(File);
    return 1;
}

stock LoadFoodStands( ){

    for(new i = 0; i < MAX_FOOD_STANDS; i++){

        new fsFile[ 40 ], str[128];
        format(fsFile, sizeof(fsFile), FSTAND_FILE, i);
        if(fexist(fsFile)){
            
            INI_ParseFile(fsFile, "LoadFoodStandData", .bExtra = true, .extra = i);

            format(str, sizeof(str), "{737BE1}Food Stand (%d)\n{FFFFFF}/buyfood", fStandInfo[i][fsID]);
            fStandLabel[i] = Create3DTextLabel(str, -1, fStandInfo[i][fstand_pos][0], fStandInfo[i][fstand_pos][1], fStandInfo[i][fstand_pos][2], 5.0, 0, 0);       
        
            __g_fscounter++;
            printf("[DEBUG]: Ucitano %d Food Standova!", __g_fscounter);
        }
    }
}

//              >> [ FUNCTIONS ] <<

stock IsPlayerNearFoodStand(playerid){

    for( new i = 0; i < MAX_FOOD_STANDS; i++) {

        if( IsPlayerInRangeOfPoint( playerid, 3.0, fStandInfo[i][fstand_pos][0], fStandInfo[i][fstand_pos][1], fStandInfo[i][fstand_pos][2] ) ) { return i; }
    }
    return INVALID_FOOD_STAND_ID;
}

stock CreateFoodStand(playerid, fsid){

    new fsFile[ 40 ];
    format(fsFile, sizeof(fsFile), FSTAND_FILE, fsid);
    if(fexist(fsFile)) return va_SendClientMessage(playerid, -1, "{737BE1}Food Stand (ID:%d) vec postoji!", fsid);

    new Float:playerPos[3], str[128];
    GetPlayerPos(playerid, playerPos[0], playerPos[1], playerPos[2]);

    fStandInfo[fsid][fsID] = fsid;
    fStandInfo[fsid][fstand_pos][0] = playerPos[0];
    fStandInfo[fsid][fstand_pos][1] = playerPos[1];
    fStandInfo[fsid][fstand_pos][2] = playerPos[2];

    format(str, 128, "{737BE1}Food Stand (%d)\n{FFFFFF}/buyfood", fsid);
    fStandLabel[fsid] = Create3DTextLabel(str, -1, playerPos[0], playerPos[1], playerPos[2], 5.0, 0, 0);

    SaveFoodStand(fsid);

    va_SendClientMessage(playerid, -1, "{737BE1}Food Stand (%d) je uspjesno kreiran!", fsid);
    return 1;
}

stock RemoveFoodStand(playerid, fsid){

    new fsFile[ 40 ];
    format(fsFile, sizeof(fsFile), FSTAND_FILE, fsid);

    if(fexist(fsFile)){

        fremove(fsFile);
        Delete3DTextLabel(fStandLabel[fsid]);
        va_SendClientMessage(playerid, -1, "{737BE1}Food Stand (ID:%d) je uspjesno obrisan!", fsid);
    }
    va_SendClientMessage(playerid, -1, "{737BE1}Food Stand (ID:%d) ne postoji!", fsid);
    return 1;
}

stock SelectFood(playerid){

    new foodstand = IsPlayerNearFoodStand(playerid);

    if(foodstand == INVALID_FOOD_STAND_ID) return SendClientMessage(playerid, -1, "{FF0000}Ne nalazite se u blizini food stand-a!");

    ShowPlayerDialog(playerid, dialog_FoodStand, DIALOG_STYLE_LIST, "{DAA520}Food Stand - {FFFFFF}Menu", "[1] > Hot Dog\n[2] > Nudle\n[3] > Sendic", "Odaberi", "Odustani");
    return 1;
}

stock FeedPlayer(playerid, food){

    new Float:health;
    GetPlayerHealth(playerid, health);
    if(food == 1){
        if(health > 95) return SendClientMessage(playerid, -1, "{FF0000}Niste gladni!");
        SetPlayerHealth(playerid, health+5);
        GivePlayerMoney(playerid, -10);
        __g_PlayerIsEating[playerid] = true;
    }
    else if(food == 2){
        if(health > 80) return SendClientMessage(playerid, -1, "{FF0000}Niste gladni!");
        SetPlayerHealth(playerid, health+20);
        GivePlayerMoney(playerid, -10);
        __g_PlayerIsEating[playerid] = true;
    }
    else if(food == 3){
        if(health > 60) return SendClientMessage(playerid, -1, "{FF0000}Niste gladni!");
        SetPlayerHealth(playerid, health+40);
        GivePlayerMoney(playerid, -25);
        __g_PlayerIsEating[playerid] = true;
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){

    if(dialogid == dialog_FoodStand){

        if(!response) return 1;
        if(response){

            switch(listitem){
                case 0:{

                    FeedPlayer(playerid, 1);
                }
                case 1:{
                    FeedPlayer(playerid, 2);
                }
                case 2:{
                    FeedPlayer(playerid, 3);
                }
            }
        }
    }
    return 1;
}

