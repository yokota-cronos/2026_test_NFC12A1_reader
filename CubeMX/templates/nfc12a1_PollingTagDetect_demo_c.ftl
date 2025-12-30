[#ftl]
[#assign useNFC12POLLINGTAGDETECT = false]


[#assign useSPI = false]

[#if RTEdatas??]
[#list RTEdatas as define]

[#if define?contains("NFC12A1_POLLING_TAG_DETECT")]
[#assign useNFC12POLLINGTAGDETECT = true]
[/#if]


[#if define?contains("USE_SPI")]
[#assign useSPI = true]
[/#if]

[/#list]
[/#if] 
void MX_NFC12_PollingDemo_Init(void)
{
  BSP_NFC0XCOMM_Init();
  BSP_COM_Init(COM1);
  logUsartInit(&hcom_uart[COM1]);
  USR_INT_LINE.Line = USR_INT_LINE_NUM;
[#if ("${McuName}"?contains("STM32G0")) || ("${McuName}"?contains("STM32U5")) || ("${McuName}"?contains("STM32U0")) || ("${McuName}"?contains("STM32C0"))]
[#else]   
  USR_INT_LINE.PendingCallback = st25r500Isr;
[/#if] 
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
     
   /* Configure interrupt callback */
  (void)HAL_EXTI_GetHandle(&USR_INT_LINE, USR_INT_LINE.Line);
  (void)HAL_EXTI_RegisterCallback(&USR_INT_LINE, HAL_EXTI_COMMON_CB_ID, BSP_NFC0XCOMM_IRQ_Callback);
   
#ifdef ST25R500
  platformLog("Welcome to X-NUCLEO-NFC12A1\r\n");
#endif /* ST25R500 */

  /* Initialize RFAL */
  if (!demoIni())
  {
    /*
    * in case the rfal initialization failed signal it by flashing all LED
    * and stopping all operations
    */
    platformLog("Initialization failed..\r\n");
    while (1) 
    {
      platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
      platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
      platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
      platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
      platformLedToogle(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);

      platformDelay(100);
    }
  } 
  else
  {
    platformLog("Initialization succeeded..\r\n");
    for (int i = 0; i < 6; i++) 
    {
      platformLedToogle(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
      platformLedToogle(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
      platformLedToogle(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
      platformLedToogle(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
      platformLedToogle(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);

      platformDelay(200);
    }
	
    platformLedOff(PLATFORM_LED_A_PORT, PLATFORM_LED_A_PIN);
    platformLedOff(PLATFORM_LED_B_PORT, PLATFORM_LED_B_PIN);
    platformLedOff(PLATFORM_LED_F_PORT, PLATFORM_LED_F_PIN);
    platformLedOff(PLATFORM_LED_V_PORT, PLATFORM_LED_V_PIN);
    platformLedOff(PLATFORM_LED_FIELD_PORT, PLATFORM_LED_FIELD_PIN);

  }
}  
  
[#if useSPI]
/**
  * @brief      SPI Read and Write byte(s) to device
  * @param[in]  pTxData : Pointer to data buffer to write
  * @param[out] pRxData : Pointer to data buffer for read data
  * @param[in]  Length : number of bytes to write
  * @return     BSP status
  */
int32_t BSP_NFC0XCOMM_SendRecv(const uint8_t *const pTxData, uint8_t *const pRxData, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_ERROR;
  int32_t ret = BSP_ERROR_NONE;
  
  if((pTxData != NULL) && (pRxData != NULL))
  {
    status = HAL_SPI_TransmitReceive(&COMM_HANDLE, (uint8_t *)pTxData, (uint8_t *)pRxData, Length, 2000);
  } 
  else if ((pTxData != NULL) && (pRxData == NULL)) 
  {
    status = HAL_SPI_Transmit(&COMM_HANDLE, (uint8_t *)pTxData, Length, 2000);
  } 
  else if ((pTxData == NULL) && (pRxData != NULL)) 
  {
    status = HAL_SPI_Receive(&COMM_HANDLE, (uint8_t *)pRxData, Length, 2000);
  }
  else
  {
  	ret = BSP_ERROR_WRONG_PARAM;
  }
  
  /* Check the communication status */
  if (status != HAL_OK)
  {
    /* Execute user timeout callback */
    ret = BSP_NFC0XCOMM_Init();
  }
  
  return ret;
}


[/#if]


/**
  * @brief  BSP SPI1 callback
  * @param  None
  * @return None
  */
__weak void BSP_NFC0XCOMM_IRQ_Callback(void)
{
  /* Prevent unused argument(s) compilation warning */
  
  /* This function should be implemented by the user application.
   * It is called into this driver when an event from ST25R500 is triggered. 
   */
  st25r500Isr();
}
