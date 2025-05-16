// am_phase_compensation.c
#include "SignalCalandGen.h"

// 全局配置（初始值归零，建议在main()中初始化）
AM_PhaseConfig g_am_phase = {0};

void AM_PhaseComp_Update(void)
{
    // 1. 计算角频率 (rad/ns)
    const float carrier_omega = 2.0f * (float)M_PI * CARRIER_FREQ * 1e-9f;
    const float modulation_omega = 2.0f * (float)M_PI * MOD_FREQ * 1e-9f;
    
    // 2. 计算各项补偿（幅度参数MOD_INDEX和RMS_VOLTAGE当前未使用）
    float delay_comp_carrier = -carrier_omega * DELAY_NS;       // 时延补偿
    float delay_comp_mod = -modulation_omega * DELAY_NS;
    
    float sys_comp_carrier = CARRIER_PHASE_ERROR(CARRIER_FREQ); // 系统误差
    float sys_comp_mod = MODULATION_PHASE_ERROR(MOD_FREQ);
    
    float init_phase_rad = INIT_PHASE_DEG * ((float)M_PI / 180.0f); // 初始相位
    
    // 3. 合并所有补偿项
    CARRIER_COMP = delay_comp_carrier + sys_comp_carrier + init_phase_rad;
    MOD_COMP = delay_comp_mod + sys_comp_mod;
    
    // 4. 相位归一化
    CARRIER_COMP = fmodf(CARRIER_COMP + (float)M_PI, 2*(float)M_PI) - (float)M_PI;
    MOD_COMP = fmodf(MOD_COMP + (float)M_PI, 2*(float)M_PI) - (float)M_PI;
    
    /* 幅度参数当前未参与计算，保留供未来扩展 */
    /* 可在此添加幅度相关处理逻辑，例如：
       if(MOD_INDEX > 1.0f) MOD_INDEX = 1.0f;
       if(RMS_VOLTAGE < 0.1f) RMS_VOLTAGE = 0.1f;
    */
}
