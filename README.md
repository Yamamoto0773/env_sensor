# env_sensor

気温，湿度，気圧，CO2濃度を測定し，Ambientに送信します．

## 測定データ

[Ambient ボード](https://ambidata.io/bd/board.html?id=17173)

## 使用センサ
- 温湿度センサ\
  SHT31-DIS
- 大気圧センサ\
  LPS331
- CO2センサ\
  MH-Z19B

## 実行方法

```sh
$ cd env_sensor
$ gcc src/main.c src/mhz19b.c src/i2c.c src/serial.c src/ambient.c -Wall
$ ./a.out <channel_id> <write_key>
```



