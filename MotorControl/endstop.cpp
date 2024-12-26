#include <odrive_main.h>


void Endstop::update() {
    debounceTimer_.update();//!更新定时器值
    last_state_ = endstop_state_;//!更新上一次的按键状态
    if (config_.enabled) {
        bool last_pin_state = pin_state_;

        pin_state_ = get_gpio(config_.gpio_num).read();

        // If the pin state has changed, reset the timer
        //!如果当前引脚状态与上一状态不同，说明引脚发生了变化，重置定时器
        if (pin_state_ != last_pin_state)
            debounceTimer_.reset();
        //!检查计时器是否到期timeout
        if (debounceTimer_.expired())//!如果去抖计时器到期，说明引脚状态稳定了，可以更新逻辑上的限位开关状态
            endstop_state_ = config_.is_active_high ? pin_state_ : !pin_state_;  // endstop_state is the logical state
    } else {
        endstop_state_ = false;
    }
}

bool Endstop::apply_config() {
    debounceTimer_.reset();
    if (config_.enabled) {
        debounceTimer_.start();
    } else {
        debounceTimer_.stop();
    }
    debounceTimer_.setIncrement(config_.debounce_ms * 0.001f);
    return true;
}
