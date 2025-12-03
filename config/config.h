// hand-written config.h for the 4x4 reference design

// it is currently assumed all expanders are on i2c0
#define ADC_CNT 1
#define ADC_ADDRS { 0x17 }
#define DRIVER_CNT 1
#define DRIVER_ADDRS { 0x20 }

#define ROWS 4
#define COLUMNS 4

// {driver, pin}
#define ROWS_MAP { { 0, 7 }, { 0, 6 }, { 0, 5 }, { 0, 4 } }

// {adc, pin}
#define COLS_MAP { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }
