/*

  - Name : Food Stands - Create a simple food stands!
  - Date : 4.9.2022
  - Github : DinoWETT
  - Credits : DEntisT
  - Note : Morate imate kreiran folder "FoodStands" u scriptfles folderu!
  - Version 1.0.1
  - Update : Dodan VirualWorld food standa i tip istog.
  - Copyright (C) all rights reserved

  - Tip štandova je na volju, nema odredjen tip npr.(Tip 1 = Štand sa pićem ili necim drugim.) daju se na volju.

*/
  

#if defined _foodstands_INCLUDED
    #endinput
#endif
#define _foodstands_INCLUDED
#include <a_samp>
#include <YSI\y_hooks>

//              >> [ DEFINES ] <<

#if defined MAX_FOOD_STANDS
    #undef MAX_FOOD_STANDS
#else
    #define MAX_FOOD_STANDS 30
#endif

#define INVALID_FOOD_STAND_ID -1

//              >> [ ENUMS ] <<

enum e_FOOD_STAND_DATA {

    Float:fstand_pos[3],
    fsType,
    fsVW
}

new fStandInfo[MAX_FOOD_STANDS][e_FOOD_STAND_DATA];

//              >> [ FUNCTIONS ] <<

forward OnFoodStandCreated(standid);

stock GetFoodStandNearPlayer(playerid){

    for( new i = 0; i < MAX_FOOD_STANDS; i++) {

        if( IsPlayerInRangeOfPoint( playerid, 3.0, fStandInfo[i][fstand_pos][0], fStandInfo[i][fstand_pos][1], fStandInfo[i][fstand_pos][2] )  && GetPlayerVirtualWorld(playerid) == fStandInfo[i][fsVW]) return i;
    }
    return INVALID_FOOD_STAND_ID;
}

stock GetFreeFoodStandID( ){

    for(new i = 0; i < MAX_FOOD_STANDS; i++){
        if( fStandInfo[i][fstand_pos][0] == 0.00 ) return i;
    }

    return INVALID_FOOD_STAND_ID;
}

stock bool:ResetFoodStandInfo(standid){

    if(standid < 0 || standid > MAX_FOOD_STANDS) return false;

    fStandInfo[standid][fstand_pos][0] = 0.00;
    fStandInfo[standid][fstand_pos][1] = 0.00;
    fStandInfo[standid][fstand_pos][2] = 0.00;
    fStandInfo[standid][fsVW] = 0;
    fStandInfo[standid][fsType] = 0;

    return true;
}

stock bool:IsPlayerNearFoodStand(playerid){

    if(!IsPlayerConnected(playerid)) return false;
    if(GetFoodStandNearPlayer(playerid) == INVALID_FOOD_STAND_ID) return false;
    return true;
}

stock CreateFoodStand(Float:cord_x, Float:cord_y, Float:cord_z, type, virtual_world){

    new standid = GetFreeFoodStandID( );
    if(standid == INVALID_FOOD_STAND_ID) return INVALID_FOOD_STAND_ID;

    fStandInfo[standid][fstand_pos][0] = cord_x;
    fStandInfo[standid][fstand_pos][1] = cord_y;
    fStandInfo[standid][fstand_pos][2] = cord_z;

    fStandInfo[standid][fsType] = type;
    fStandInfo[standid][fsVW] = virtual_world;

    CallRemoteFunction("OnFoodStandCreated", "d", standid);

    return standid;
}


hook OnGameModeInit( ){

    for(new i = 0; i < MAX_FOOD_STANDS; i++){

        ResetFoodStandInfo( i );
    }

    IsPlayerNearFoodStand(0);

    return 1;
}

