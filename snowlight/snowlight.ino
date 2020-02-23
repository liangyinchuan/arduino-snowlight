#include "SoftPWM.h"
#include <ADCTouchSensor.h>

byte edgeLedPins[] = {12,13,18,19,2,8};
byte middleLedPins[] = {11,10,9,5,3,6};
byte innerLedPins[] = {14,16,17,15,4,7};

ADCTouchSensor touchSensor = ADCTouchSensor(A7, -1); 

void setup() {
    Serial.begin(9600);
    SoftPWMBegin();
    touchSensor.begin();

}

byte animation = 0;
long touchAt = 0;

void loop() {
    if (Serial.available()){
        int t = Serial.parseInt();
        if (t>0)
        {
            animation = t;
        }
    }

    Serial.println(animation);
    switch (animation) {
        case 0:
            breathing();
            animation = 1;
        break;

        case 1:
            circleAnimation();
        break;

        case 2:
            shinyAnimation();
        break;

        case 3:
            loopAnimation();
        break;

        case 4:
            fireworkAnimation();
        break;

        case 5:
            smileAnimation();
        break;

        case 6:
            waveAnimation();
        break;

        case 7:
            slowOnAnimation();
        break;

        case 8:
            fadingAnimation();
        break;

        default:
            animation = 0;
        break;
    }

    int touchValue = touchSensor.read();
    if (touchAt + 1000 < millis() && touchValue > 100) {
        touchAt = millis(); // touch down, cold down timeout is 2s
        animation ++;
        _fill(0);
    }

}


byte circleState[] = {10, 5, 1};
byte circleStateAnimation[] = {1, 1, 1};

// 圆周运动模式
void circleAnimation() {
    for (int i = 0; i < 3; i++) {

        if (circleState[i] >= 10) {
            circleStateAnimation[i] = -1; // dim
        }else if (circleState[i] <= 1) {
            circleStateAnimation[i] = 1; // bright
        }
        circleState[i] += circleStateAnimation[i];
    }
    for (int i = 0; i < 6; i++) {
        SoftPWMSet(innerLedPins[i], circleState[0]);
        SoftPWMSet(middleLedPins[i], circleState[1]);
        SoftPWMSet(edgeLedPins[i], circleState[2]);
    }
    delay(100);
}

byte waveState[] = {60, 30, 10, 5, 30, 60};
byte waveStateAnimation[] = {1, 1, 1, -1, -1, -1};
// 波形动画
void waveAnimation() {
    for (int i = 0; i < 6; i++) {
        if (waveState[i] >= 60) {
            waveStateAnimation[i] = -1; // dim
        }else if (waveState[i] <= 5) {
            waveStateAnimation[i] = 1; // bright
        }
        waveState[i] += waveStateAnimation[i];
    }
    for (int i = 0; i < 6; i+=2) {
        SoftPWMSet(innerLedPins[i], waveState[0]);
        SoftPWMSet(middleLedPins[i], waveState[1]);
        SoftPWMSet(edgeLedPins[i], waveState[2]);
        SoftPWMSet(innerLedPins[i + 1], waveState[3]);
        SoftPWMSet(middleLedPins[i + 1], waveState[4]);
        SoftPWMSet(edgeLedPins[i + 1], waveState[5]);
    }
    delay(10);
}

// 火线动画模式
void fireworkAnimation() {
    int delayTime = 80;
    for (int i = 0; i < 6; i++) {
        SoftPWMSet(innerLedPins[i], 5);
        delay(delayTime);
    }
    for (int i = 0; i < 6; i++) {
        // Serial.println((i+1)%6);
        SoftPWMSet(innerLedPins[i], 0);
        SoftPWMSet(middleLedPins[(i+1)%6], 150);
        delay(delayTime);
    }
    for (int i = 0; i < 6; i++) {
        SoftPWMSet(middleLedPins[(i+1)%6], 0);
        SoftPWMSet(edgeLedPins[(i+1)%6], 200);
        delay(delayTime);
    }

    for (int i = 0; i < 6; i++) {
        SoftPWMSet(innerLedPins[i], 5);
        delay(delayTime);
    }
    for (int i = 0; i < 6; i++) {
        // Serial.println((i+1)%6);
        SoftPWMSet(innerLedPins[i], 0);
        SoftPWMSet(middleLedPins[(i+1)%6], 150);
        delay(delayTime);
    }
    for (int i = 0; i < 6; i++) {
        SoftPWMSet(innerLedPins[i], 5);
        delay(delayTime);
    }
    for (int i = 5; i >= 0; i--) {
        SoftPWMSet(edgeLedPins[(i+1)%6], 0);
        delay(delayTime);
    }

    for (int i = 5; i >= 0; i--) {
        SoftPWMSet(middleLedPins[(i+1)%6], 0);
        delay(delayTime);
    }

    for (int i = 5; i >= 0; i--) {
        SoftPWMSet(innerLedPins[(i+1)%6], 0);
        delay(delayTime);
    }
    delay(100);
}

int initLd = 10;
int minLd = 1;
// smile 模式
void smileAnimation() {
    // 大嘴初始化
    SoftPWMSet(innerLedPins[0], initLd);
    SoftPWMSet(innerLedPins[1], initLd);
    SoftPWMSet(innerLedPins[5], initLd);
    // 眼睛初始化
    SoftPWMSet(innerLedPins[3], initLd);

    // 外层初始化
    int wld = 200;
    for (int i = 0; i < 6; i++){
        SoftPWMSet(middleLedPins[i],wld);
        SoftPWMSet(edgeLedPins[i],wld);
    }
    delay(1000);
    // 眨眼动作
    blink(2);

    // 说话
    say(4);
}

// 眨眼
void blink(int times){
    for (int i = 0; i < times; i++)
    {
        for (int i = initLd; i >= minLd; i--){
            SoftPWMSet(innerLedPins[3], i);
            delay(1);
        }
        for (int i = minLd; i <= initLd; i++){
            SoftPWMSet(innerLedPins[3], i);
            delay(3);
        }
    }
}

// 说话
void say(int times){
    bool right = true;
    for (int i = 0; i < 3; ++i){
        if (right){
            SoftPWMSet(innerLedPins[1], 0);
            SoftPWMSet(innerLedPins[5], 0);
        }else{
            SoftPWMSet(innerLedPins[1], initLd);
            SoftPWMSet(innerLedPins[5], initLd);
        }
        delay(200);
        right = !right;
    }
}



// 呼吸灯
void breathing(){
    // 定义配置 亮度 延时
    // 最大亮度
    int max_luminance = 12;
    // 延时
    int delayed = 70;
    // 变暗最低亮度
    int min_luminance = 2;

    int i = min_luminance;
    for(i; i <= max_luminance; i++){
        for (int j = 0; j < 6; j++){
            SoftPWMSet(edgeLedPins[j], i);
            SoftPWMSet(middleLedPins[j], i);
            SoftPWMSet(innerLedPins[j], i);
        }
        delay(delayed);
    }

    for(i; i >= min_luminance; i--){
        for (int j = 0; j < 6; j++){
            SoftPWMSet(edgeLedPins[j], i);
            SoftPWMSet(middleLedPins[j], i);
            SoftPWMSet(innerLedPins[j], i);
        }
        delay(delayed);
    }
}

byte loopCounter = 0;
byte loopState = 100;
byte loopState_inner = 5;

// 循环动画
void loopAnimation() {
    SoftPWMSet(innerLedPins[loopCounter], loopState_inner);
    SoftPWMSet(middleLedPins[loopCounter], loopState);
    SoftPWMSet(edgeLedPins[loopCounter], loopState);

    loopCounter = _nextIndex(loopCounter, 1);
    if (loopCounter == 0) {
        loopState = (loopState == 100 ? 0 : 100);
        loopState_inner = (loopState_inner == 5 ? 0 : 5);
    }
    delay(100);
}

// 慢声动画
void slowOnAnimation() {
    byte randomLed = random(0, 18);
    int ld = random(1,5);
    if (randomLed < 6) {
        SoftPWMSet(innerLedPins[randomLed], ld%4);
    }else if (randomLed < 12) {
        SoftPWMSet(middleLedPins[randomLed - 6], ld%4);
    }else {
        SoftPWMSet(edgeLedPins[randomLed - 12], ld%4);
    }
    delay(50);
}


byte shinyState[] = {0, 100, 0, 100, 0, 100};
byte shinyStateAnimation[] = {1, 1, 1, 1, 1, 1};
byte shinyCounter = 0;

// 神谕
void shinyAnimation() {
    for (int i = 0; i < 6; i++) {
        if (shinyState[i] >= 50) {
            shinyStateAnimation[i] = -1; // dim
        }else if (shinyState[i] <= 0) {
            shinyStateAnimation[i] = 1; // bright
        }
        shinyState[i] += shinyStateAnimation[i];
        SoftPWMSet(edgeLedPins[i], shinyState[i]);
    }
    shinyCounter ++;
    if (shinyCounter > 10) {
        shinyCounter = 0;
        for (byte r = random(1, 3); r > 0; r--) {
            byte randomLed = random(0, 12);
            if (randomLed < 6) {
                SoftPWMSet(innerLedPins[random(0, 6)], 5);
            }else {
                SoftPWMSet(middleLedPins[random(0, 6)], 150);
            }
        }
    }else {
        for (int i = 0; i < 6; i++) {
            SoftPWMSet(innerLedPins[i], 10);
            SoftPWMSet(middleLedPins[i], 50);
        }
    }
    delay(30);
}

byte fadingState[] = {0, 10, 0, 10, 0, 10};
byte fadingStateAnimation[] = {1, 1, 1, 1, 1, 1};

// 淡入动画
void fadingAnimation() {
    for (int i = 0; i < 6; i++) {
        if (fadingState[i] >= 10) {
            fadingStateAnimation[i] = -1; // dim
        }else if (fadingState[i] <= 0) {
            fadingStateAnimation[i] = 1; // bright
        }
        fadingState[i] += fadingStateAnimation[i];
        SoftPWMSet(edgeLedPins[i], fadingState[i]);
        SoftPWMSet(middleLedPins[_nextIndex(i, 1)], fadingState[i]);
        SoftPWMSet(innerLedPins[i], fadingState[i]);

    }
    delay(50);
}

// 重置LED状态
void _fill(byte value) {
    for (int i = 0; i < 6; i++) {
        SoftPWMSet(edgeLedPins[i], value);
        SoftPWMSet(middleLedPins[i], value);
        SoftPWMSet(innerLedPins[i], value);
    }
}

byte _prevIndex(short index, byte step) {
    index -= step;
    while (index < 0) {
        index += 6;
    }
    return index;
}

byte _nextIndex(short index, byte step) {
    index += step;
    while (index > 5) {
        index -= 6;
    }
    return index;
}

