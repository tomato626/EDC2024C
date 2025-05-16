#include "gui.h"

// 环形缓冲，存放按行分割后的消息
static char msgBuffer[MAX_LINES][MAX_COLS + 1];
// 标记每行是否为一条消息的首行，以及该前缀长度和颜色
static uint8_t isPrefixLine[MAX_LINES];
static uint8_t prefixLenBuf[MAX_LINES];
static uint32_t prefixColorBuf[MAX_LINES];

static int  bufStart = 0;    // 环形缓冲起始索引
static int  bufCount = 0;    // 当前缓存行数




/**
 * @brief  将新的行推入缓冲，若超出则丢弃最老的一行
 */
static void push_line(const char *line, uint8_t isPrefix, uint8_t pfxLen, uint32_t pfxColor)
{
    int idx;
    if (bufCount < MAX_LINES) {
        idx = (bufStart + bufCount) % MAX_LINES;
        bufCount++;
    } else {
        idx = bufStart;
        bufStart = (bufStart + 1) % MAX_LINES;
    }
    strncpy(msgBuffer[idx], line, MAX_COLS);
    msgBuffer[idx][MAX_COLS] = '\0';
    isPrefixLine[idx] = isPrefix;
    prefixLenBuf[idx] = pfxLen;
    prefixColorBuf[idx] = pfxColor;
}

/**
 * @brief  重绘整个缓冲区到屏幕，只在消息尾绘制分隔线，并为每条消息首行前缀着色
 */
static void redraw_all(void)
{
    LCD_SetBackColor(LCD_WHITE);
    LCD_Clear();
    LCD_SetAsciiFont(&ASCII_Font20);

    for (int i = 0; i < bufCount; i++) {
        int idx   = (bufStart + i) % MAX_LINES;
        int lineY = i * FONT20_HEIGHT_PIXELS;

        // 绘制文本（默认黑色全行）
        LCD_SetColor(LCD_BLACK);
        LCD_DisplayString(0, lineY, msgBuffer[idx]);

        // 如果是首行，重绘前缀为对应颜色
        if (isPrefixLine[idx]) {
            char pfx[MAX_COLS + 1] = {0};
            memcpy(pfx, msgBuffer[idx], prefixLenBuf[idx]);
            pfx[prefixLenBuf[idx]] = '\0';
            LCD_SetColor(prefixColorBuf[idx]);
            LCD_DisplayString(0, lineY, pfx);
        }

        // 如果下一行是首行或已到最后一行，则此行为消息尾，绘制分隔线
        int nextBufIndex = (idx + 1) % MAX_LINES;
        int nextI = i + 1;
        if (nextI == bufCount || isPrefixLine[nextBufIndex]) {
            int y_sep = lineY + FONT20_HEIGHT_PIXELS - 1;
            LCD_SetColor(LCD_BLACK); // 保持分隔线为黑色
            LCD_DrawLine(0, y_sep, SCREEN_WIDTH_PIXELS - 1, y_sep);
        }
    }
}

/**
 * @brief  带彩色前缀、自动换行、滚动的打印函数
 * @param  prefix:      前缀字符串 (如 "SYS")
 * @param  prefixColor: 前缀颜色 (uint32_t, e.g. LCD_RED)
 * @param  msg:         英文消息体
 */
void SPI_PrintWithPrefix(const char *prefix, uint32_t prefixColor, const char *msg)
{
    // 构造完整字符串：prefix + ":" + msg
    size_t pfxLen = strlen(prefix) + 1; // 含 ':'
    size_t msgLen = strlen(msg);
    char *full = malloc(pfxLen + msgLen + 1);
    strcpy(full, prefix);
    strcat(full, ":");
    strcat(full, msg);

    // 分行存入缓冲，标记首行前缀
    size_t pos = 0;
    uint8_t firstLine = 1;
    while (pos < strlen(full)) {
        size_t rem = strlen(full) - pos;
        size_t len = (rem < MAX_COLS ? rem : MAX_COLS);
        char line[MAX_COLS + 1];
        memcpy(line, full + pos, len);
        line[len] = '\0';

        if (firstLine) {
            push_line(line, 1, pfxLen, prefixColor);
            firstLine = 0;
        } else {
            push_line(line, 0, 0, 0);
        }
        pos += len;
    }
    free(full);

    // 重绘所有内容
    redraw_all();
}

void syslog(const char *msg){
  SPI_PrintWithPrefix("SYS", LCD_RED, msg);
}
