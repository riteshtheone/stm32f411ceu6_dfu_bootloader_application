/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "gpio.h"
#include "usart.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
typedef struct {
    __IO uint32_t flag, reserved;
} boot_flag_t;

#define APP_ADDR (0x08008000UL)

#define BOOT_FLAG_ADDR (0x20000000UL)
#define APP_FLAG (0xDEADBEEFUL)  // Signal: Jump to Application
#define DFU_FLAG (0xA5A5A5A5UL)  // Signal: Enter Firmware Update mode

#define RESERVED (0x0U)

#define APP_SP (*(__IO uint32_t *)(APP_ADDR))         // app stack pointer addr
#define APP_ENTRY (*(__IO uint32_t *)(APP_ADDR + 4))  // app reset handler addr

#define BOOT_FLAG_PTR ((boot_flag_t *)BOOT_FLAG_ADDR)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void _halt() {
    __disable_irq();
    while (1) {
        __WFI();
    }
}

void delay(uint32_t ms) {
    uint32_t _wait = uwTick + ms;
    while ((int32_t)(uwTick - _wait) < 0) {
        __NOP();
    }
}

void set_boot_flag(uint32_t flag) {
    BOOT_FLAG_PTR->flag = flag;
    BOOT_FLAG_PTR->reserved = RESERVED;
}

void check_app_vector_table(void) {
    uint8_t is_app_sp_valid = (APP_SP >= SRAM_BASE) && (APP_SP <= SRAM_BASE + 128 * 1024) && ((APP_SP & 0x3UL) == 0x0U);
    uint8_t is_app_entry_valid = (APP_ENTRY >= APP_ADDR) && (APP_ENTRY <= FLASH_END);
    uint8_t is_thumb_mode = (APP_ENTRY & 0x1UL) == 0x1UL;

    uint8_t is_app_present = is_app_sp_valid && is_app_entry_valid && is_thumb_mode;

    // #define COLOR_RESET "\x1B[0m"
    // #define COLOR_BORDER "\x1B[35m"  // Magenta for borders
    // #define COLOR_HEADER "\x1B[36m"  // Cyan for section headers
    // #define COLOR_LABEL "\x1B[37m"   // White/gray for field names
    // #define COLOR_ADDR "\x1B[33m"    // Yellow for addresses
    // #define COLOR_TRUE "\x1B[32m"    // Green for true
    // #define COLOR_FALSE "\x1B[31m"   // Red for false
    //
    // #define BOOL_COLOR(b) ((b) ? COLOR_TRUE "true " COLOR_RESET : COLOR_FALSE "false" COLOR_RESET)
    //
    //     printf(COLOR_BORDER "┌──────────────────────────────────────────────┐\n" COLOR_RESET);
    //     printf(COLOR_BORDER "│" COLOR_HEADER "               Bootloader Info                " COLOR_BORDER "│\n" COLOR_RESET);
    //     printf(COLOR_BORDER "├──────────────────────────────────────────────┤\n" COLOR_RESET);
    //
    //     printf(COLOR_BORDER "│" COLOR_LABEL " Boot_Flag.flag        : " COLOR_ADDR "0x%08lX" COLOR_BORDER "           │\n" COLOR_RESET, BOOT_FLAG_PTR->flag);
    //     printf(COLOR_BORDER "│" COLOR_LABEL " Boot_Flag.reserved    : " COLOR_ADDR "0x%08lX" COLOR_BORDER "           │\n" COLOR_RESET, BOOT_FLAG_PTR->reserved);
    //     printf(COLOR_BORDER "│                                              │\n" COLOR_RESET);
    //
    //     printf(COLOR_BORDER "│" COLOR_HEADER " App Vector Table:                            " COLOR_BORDER "│\n" COLOR_RESET);
    //     printf(COLOR_BORDER "│" COLOR_LABEL "   APP_SP              : " COLOR_ADDR "0x%08lX" COLOR_BORDER "           │\n" COLOR_RESET, APP_SP);
    //     printf(COLOR_BORDER "│" COLOR_LABEL "   APP_ENTRY           : " COLOR_ADDR "0x%08lX" COLOR_BORDER "           │\n" COLOR_RESET, APP_ENTRY);
    //     printf(COLOR_BORDER "│                                              │\n" COLOR_RESET);
    //
    //     printf(COLOR_BORDER "│" COLOR_HEADER " User Application Details:                    " COLOR_BORDER "│\n" COLOR_RESET);
    //     printf(COLOR_BORDER "│" COLOR_LABEL "   Stack Pointer in RAM  : %s" COLOR_BORDER "              │\n" COLOR_RESET, BOOL_COLOR(is_app_sp_valid));
    //     printf(COLOR_BORDER "│" COLOR_LABEL "   Entry Point in Flash  : %s" COLOR_BORDER "              │\n" COLOR_RESET, BOOL_COLOR(is_app_entry_valid));
    //     printf(COLOR_BORDER "│" COLOR_LABEL "   Entry is Thumb Mode   : %s" COLOR_BORDER "              │\n" COLOR_RESET, BOOL_COLOR(is_thumb_mode));
    //     printf(COLOR_BORDER "│                                              │\n" COLOR_RESET);
    //
    //     printf(COLOR_BORDER "│" COLOR_HEADER " => User Application Available : %s" COLOR_BORDER "        │\n" COLOR_RESET, BOOL_COLOR(is_app_present));
    //     printf(COLOR_BORDER "└──────────────────────────────────────────────┘\n" COLOR_RESET);

#define BOOL_STR(bool) ((bool) ? "true" : "false")
    printf("\n+---------------------------------------------+\n");
    printf("|               Bootloader Info               |\n");
    printf("+---------------------------------------------+\n");

    printf("| Boot Flag .flag       : 0x%08lX          |\n", BOOT_FLAG_PTR->flag);
    printf("| Boot Flag .reserved   : 0x%08lX          |\n", BOOT_FLAG_PTR->reserved);
    printf("|                                             |\n");

    printf("| User Application Vector Table:              |\n");
    printf("|   Application Stack   : 0x%08lX          |\n", APP_SP);
    printf("|   Application Entry   : 0x%08lX          |\n", APP_ENTRY);
    printf("|                                             |\n");

    printf("| User Application Details:                   |\n");
    printf("|   Stack Pointer in RAM : %-5s              |\n", BOOL_STR(is_app_sp_valid));
    printf("|   Entry Point in Flash : %-5s              |\n", BOOL_STR(is_app_entry_valid));
    printf("|   MCU in Thumb Mode    : %-5s              |\n", BOOL_STR(is_thumb_mode));
    printf("|                                             |\n");

    printf("| => User Application Valid : %-5s           |\n", BOOL_STR(is_app_present));
    printf("+--------------------xxx----------------------+\n\n");
}

uint8_t is_app_vector_table_valid(void) {
    uint8_t is_app_sp_valid = (APP_SP >= SRAM_BASE) && (APP_SP <= SRAM_BASE + 128 * 1024) && ((APP_SP & 0x3UL) == 0x0U);
    uint8_t is_app_entry_valid = (APP_ENTRY >= APP_ADDR) && (APP_ENTRY <= FLASH_END);
    uint8_t is_thumb_mode = (APP_ENTRY & 0x1UL) == 0x1UL;

    return is_app_sp_valid && is_app_entry_valid && is_thumb_mode;
}

void jump_to_application(void) {
    __set_MSP(APP_SP);

    SCB->VTOR = APP_ADDR;
    ((void (*)(void))APP_ENTRY)();

    _halt();
}

void check_boot_flag() {
    if (BOOT_FLAG_PTR->flag == APP_FLAG && BOOT_FLAG_PTR->reserved == RESERVED) {
        set_boot_flag(RESERVED);
        jump_to_application();
    }
}

void check_firmware_upgrade_btn(void) {
    if (BOOT_FLAG_PTR->flag == DFU_FLAG && BOOT_FLAG_PTR->reserved == RESERVED) {
        set_boot_flag(RESERVED);
        return;
    }

    if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET) {
        printf("Button is pressed, Waiting for realease...\n");
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
            delay(10);
        }
        delay(50);  // debounce wait...
        printf("Button is released, Entering in DFU mode...\n");

        set_boot_flag(DFU_FLAG);
    } else {
        printf("Booting User Application...\n");

        if (!is_app_vector_table_valid()) {
            delay(500);
            printf("Failed, User Application is not available...\n");

            // delay(50);
            Error_Handler();
        }

        set_boot_flag(APP_FLAG);
    }

    delay(10);
    NVIC_SystemReset();
}

void print_boot_flag(void) {
    printf("Boot_Flag flag    : 0x%08lX\n", BOOT_FLAG_PTR->flag);
    printf("Boot_Flag reserved: 0x%08lX\n", BOOT_FLAG_PTR->reserved);
}

void print_boot_flag_bytes(void) {
    __IO uint8_t *boot_flag_bytes = (__IO uint8_t *)BOOT_FLAG_ADDR;

    printf("BOOT_FLAG_PTR 8-byte region at 0x20000000:\n");

    for (int i = 0; i < 8; ++i) {
        printf("0x%08lX: 0x%02X\n", BOOT_FLAG_ADDR + i, boot_flag_bytes[i]);
    }
}

typedef struct {
    uint32_t interval;
    uint32_t last_tick;
} TimedTask;

uint8_t is_time_elapsed(TimedTask *task, uint32_t now) {
    if ((int32_t)(now - task->last_tick) >= (int32_t)task->interval) {
        task->last_tick = now;
        return 1;
    }
    return 0;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
    /* USER CODE BEGIN 1 */
    check_boot_flag();
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick.
     */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    MX_GPIO_Init();
    MX_USART1_UART_Init();

    check_app_vector_table();
    check_firmware_upgrade_btn();

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_USB_DEVICE_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    TimedTask led_blink = {.interval = 100, .last_tick = 0};
    TimedTask print_tick = {.interval = 1000, .last_tick = 0};

    while (1) {
        uint32_t _now = uwTick;

        if (is_time_elapsed(&led_blink, _now)) {
            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        }

        if (is_time_elapsed(&print_tick, _now)) {
            printf("Boot_Tick %lu\n", _now / print_tick.interval);
        }

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    // __disable_irq();
    //
    TimedTask err_led_blink = {.interval = 1000, .last_tick = 0};

    while (1) {
        uint32_t _now = uwTick;

        if (is_time_elapsed(&err_led_blink, _now)) {
            static uint8_t _blink_count = 0;

            HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
            _blink_count++;

            if (_blink_count >= 4) {            // 2 blinks = 4 toggles (ON-OFF-ON-OFF)
                err_led_blink.interval = 1500;  // Long pause
                _blink_count = 0;
            } else {
                err_led_blink.interval = 80;  // Short blink
            }
        }
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
       line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
