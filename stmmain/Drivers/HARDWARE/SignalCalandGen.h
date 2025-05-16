// SignalCalandGen.h
#ifndef __SIGNALCALANDGEN_H
#define __SIGNALCALANDGEN_H

#include "main.h"
#include <stdbool.h>
#include <math.h>

/* 系统相位误差模型（需根据实测修改）*/
#define CARRIER_PHASE_ERROR(freq)   (0.1f * (freq) / 1e6f)    // 示例：0.1rad/MHz
#define MODULATION_PHASE_ERROR(freq) (0.05f * (freq) / 1e3f)  // 示例：0.05rad/kHz

/* 全局参数声明 */
typedef struct {
    // 频率相关参数
    float carrier_freq_hz;    // 载波频率(Hz)
    float modulation_freq_hz; // 调制频率(Hz)
    
    // 时间相关参数
    float delay_ns;           // 时延(ns)
    
    // 相位相关参数
    float init_phase_deg;     // 初始相位(度)
    
    // 幅度相关参数（保留未使用）
    float modulation_index;   // 调制度(0.0-1.0)
    float rms_voltage;       // 有效值电压(100mV-1V)
    
    // 计算结果
    float carrier_phase_comp;    // 载波总补偿(rad)
    float modulation_phase_comp; // 调制信号总补偿(rad)
} AM_PhaseConfig;

extern AM_PhaseConfig g_am_phase;

/* 核心功能函数 */
void AM_PhaseComp_Update(void);

/* 快捷访问宏 */
#define CARRIER_FREQ      (g_am_phase.carrier_freq_hz)
#define MOD_FREQ          (g_am_phase.modulation_freq_hz)
#define DELAY_NS          (g_am_phase.delay_ns)
#define INIT_PHASE_DEG    (g_am_phase.init_phase_deg)
#define MOD_INDEX         (g_am_phase.modulation_index)
#define RMS_VOLTAGE       (g_am_phase.rms_voltage)
#define CARRIER_COMP      (g_am_phase.carrier_phase_comp)
#define MOD_COMP          (g_am_phase.modulation_phase_comp)

#endif
