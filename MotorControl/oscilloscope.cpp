
#include "oscilloscope.hpp"

// if you use the oscilloscope feature you can bump up this value
#define OSCILLOSCOPE_SIZE 4096

void Oscilloscope::update() {
    float trigger_data = trigger_src_ ? *trigger_src_ : 0.0f;//!触发信号
    float trigger_threshold = trigger_threshold_;//!触发阈值
    float sample_data = data_src_ ? **data_src_ : 0.0f;//!采样数据

    if (trigger_data < trigger_threshold) {
        ready_ = true;//!表示已经准备好开始采集数据
    }
    if (ready_ && trigger_data >= trigger_threshold) {
        capturing_ = true;//!开始采集数据
        ready_ = false;
    }
    if (capturing_) {
        if (pos_ < OSCILLOSCOPE_SIZE) {//!采集4096次数据，并存放在data_中
            data_[pos_++] = sample_data;
        } else {
            pos_ = 0;
            capturing_ = false;
        }
    }
}
