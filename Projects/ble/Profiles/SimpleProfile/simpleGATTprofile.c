/**************************************************************************************************
  Filename:       simpleGATTprofile.c
  Revised:        $Date: 2013-05-06 13:33:47 -0700 (Mon, 06 May 2013) $
  Revision:       $Revision: 34153 $

  Description:    This file contains the Simple GATT profile sample GATT service 
                  profile for use with the BLE sample application.

  Copyright 2010 - 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS?WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, 
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com. 
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"

#include "simpleGATTprofile.h"
#include <string.h>
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        24

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
CONST uint8 simpleProfileServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_SERV_UUID), HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
CONST uint8 simpleProfilechar1UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR1_UUID), HI_UINT16(SIMPLEPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
CONST uint8 simpleProfilechar2UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR2_UUID), HI_UINT16(SIMPLEPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
CONST uint8 simpleProfilechar3UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR3_UUID), HI_UINT16(SIMPLEPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
CONST uint8 simpleProfilechar4UUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR4_UUID), HI_UINT16(SIMPLEPROFILE_CHAR4_UUID)
};



/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static simpleProfileCBs_t *simpleProfile_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

/*
1: zero		int32	rw
2: k		float	rw
3: weight 	int32	ro
4. ad		int32	ro

*/
// Simple Profile Service attribute
static CONST gattAttrType_t simpleProfileService = { ATT_BT_UUID_SIZE, simpleProfileServUUID };


// Simple Profile Characteristic 1 Properties
static uint8 Props_Zero = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 1 Value
static int32 Value_Zero = 0;

// Simple Profile Characteristic 1 User Description
static uint8 UserDesp_Zero[5] = "zero\0";


// Simple Profile Characteristic 2 Properties
static uint8 Props_K = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 2 Value
static float Value_K = 0;

// Simple Profile Characteristic 2 User Description
static uint8 UserDesp_K[7] = "calibK\0";


static uint8 Props_Weight = GATT_PROP_READ;

// Characteristic 3 Value
static int32 Value_Weight = 0;

// Simple Profile Characteristic 3 User Description
static uint8 UserDesp_Weight[7] = "weight\0";

static uint8 Props_AD = GATT_PROP_READ;

// Characteristic 3 Value
static int32 Value_AD = 0;

// Simple Profile Characteristic 3 User Description
static uint8 UserDesp_AD[3] = "AD\0";


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&simpleProfileService            /* pValue */
  },

    // Characteristic 1 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &Props_Zero 
    },

      // Characteristic Value 1
      { 
        { ATT_BT_UUID_SIZE, simpleProfilechar1UUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8*)&Value_Zero 
      },

      // Characteristic 1 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        UserDesp_Zero 
      },      

    // Characteristic 2 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &Props_K 
    },

      // Characteristic Value 2
      { 
        { ATT_BT_UUID_SIZE, simpleProfilechar2UUID },
	GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
        0, 
        (uint8*)&Value_K 
      },

      // Characteristic 2 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        UserDesp_K
      },           
      
    // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &Props_Weight
    },

      // Characteristic Value 3
      { 
        { ATT_BT_UUID_SIZE, simpleProfilechar3UUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8*)&Value_Weight
      },

      // Characteristic 3 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        UserDesp_Weight
      },

		  // Characteristic 3 Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &Props_AD
    },

      // Characteristic Value 4
      { 
        { ATT_BT_UUID_SIZE, simpleProfilechar4UUID },
        GATT_PERMIT_READ, 
        0, 
        (uint8*)&Value_AD
      },

      // Characteristic 4 User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        UserDesp_AD
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static uint8 simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen );
static bStatus_t simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset );

static void simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType );


/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t simpleProfileCBs =
{
  simpleProfile_ReadAttrCB,  // Read callback function pointer
  simpleProfile_WriteAttrCB, // Write callback function pointer
  NULL                       // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService( uint32 services )
{
  uint8 status = SUCCESS;


  // Register with Link DB to receive link status change callback
  VOID linkDB_Register( simpleProfile_HandleConnStatusCB );  
  
  if ( services & SIMPLEPROFILE_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( simpleProfileAttrTbl, 
                                          GATT_NUM_ATTRS( simpleProfileAttrTbl ),
                                          &simpleProfileCBs );
  }

  return ( status );
}


/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCBs( simpleProfileCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    simpleProfile_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
  

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      if ( len == sizeof ( int32 ) ) 
      {
        Value_Zero= *((int32*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR2:
      if ( len == sizeof ( float ) ) 
      {
        Value_K = *((float*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR3:
      if ( len == sizeof ( int32 ) ) 
      {
        Value_Weight= *((int32*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SIMPLEPROFILE_CHAR4:
      if ( len == sizeof ( int32 ) ) 
      {
        Value_AD= *((int32*)value);   
        Value_Weight = (int)(Value_AD-Value_Zero)*Value_K;
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;
    
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case SIMPLEPROFILE_CHAR1:
      *((int32*)value) = Value_Zero;
      break;

    case SIMPLEPROFILE_CHAR2:
      *((float*)value) = Value_K;
      break;      

    case SIMPLEPROFILE_CHAR3:
      *((int32*)value) = Value_Weight;
      break;  

    case SIMPLEPROFILE_CHAR4:
      *((int32*)value) = Value_AD;
      break;
    
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 *
 * @return      Success or Failure
 */
static uint8 simpleProfile_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                            uint8 *pValue, uint8 *pLen, uint16 offset, uint8 maxLen )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
      // gattserverapp handles those reads

      // characteristics 1 and 2 have read permissions
      // characteritisc 3 does not have read permissions; therefore it is not
      //   included here
      // characteristic 4 does not have read permissions, but because it
      //   can be sent as a notification, it is included here
      case SIMPLEPROFILE_CHAR1_UUID:
      case SIMPLEPROFILE_CHAR2_UUID:
      case SIMPLEPROFILE_CHAR3_UUID:
      case SIMPLEPROFILE_CHAR4_UUID:
        *pLen = sizeof(int32);
        memcpy(pValue, pAttr->pValue, sizeof(int32));
        break;
    
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 *
 * @return  Success or Failure
 */
static bStatus_t simpleProfile_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                 uint8 *pValue, uint8 len, uint16 offset )
{
  bStatus_t status = SUCCESS;
  uint8 notifyApp = 0xFF;
  
  // If attribute permissions require authorization to write, return error
  if ( gattPermitAuthorWrite( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case SIMPLEPROFILE_CHAR1_UUID:
      case SIMPLEPROFILE_CHAR2_UUID:
     
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;   
          if(pCurValue != NULL && len == sizeof(int32))
          {
                memcpy(pCurValue, pValue, len);
                if(uuid == SIMPLEPROFILE_CHAR1_UUID)
                {
                   
                }
                else if(uuid == SIMPLEPROFILE_CHAR2_UUID)
                {
                  //Value_K = Value_K+1;
                }
          }
		  
         
        }
             
        break;
        
  
      case GATT_CLIENT_CHAR_CFG_UUID:
        status = GATTServApp_ProcessCCCWriteReq( connHandle, pAttr, pValue, len,
                                                 offset, GATT_CLIENT_CFG_NOTIFY );
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }

  // If a charactersitic value changed then callback function to notify application of change
  if ( (notifyApp != 0xFF ) && simpleProfile_AppCBs && simpleProfile_AppCBs->pfnSimpleProfileChange )
  {
    simpleProfile_AppCBs->pfnSimpleProfileChange( notifyApp );  
  }
  
  return ( status );
}

/*********************************************************************
 * @fn          simpleProfile_HandleConnStatusCB
 *
 * @brief       Simple Profile link status change handler function.
 *
 * @param       connHandle - connection handle
 * @param       changeType - type of change
 *
 * @return      none
 */
static void simpleProfile_HandleConnStatusCB( uint16 connHandle, uint8 changeType )
{ 
  // Make sure this is not loopback connection
  if ( connHandle != LOOPBACK_CONNHANDLE )
  {
    // Reset Client Char Config if connection has dropped
    if ( ( changeType == LINKDB_STATUS_UPDATE_REMOVED )      ||
         ( ( changeType == LINKDB_STATUS_UPDATE_STATEFLAGS ) && 
           ( !linkDB_Up( connHandle ) ) ) )
    { 
      //GATTServApp_InitCharCfg( connHandle, simpleProfileChar4Config );
    }
  }
}


/*********************************************************************
*********************************************************************/
