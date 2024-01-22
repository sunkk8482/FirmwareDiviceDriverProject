# 펌웨어및디바이스드라이버 프로젝트

## 개요
- LED 및 논리 게이트를 이용하여 신호등 구현

## 동작
- 4거리 신호등 중 붙어 있는 3방향의 신호
- 일정한 시간이 흐르면 다음 step으로 넘어감
- 한 방향 기준으로 파란불(7초) -> 노란불(2초) -> 빨간불/좌회전(7초) -> 빨간불/노란불(2초) -> 빨간불(18초)
- 1set가 지날시 메인보드 led에 바이너리 카운트 표시  
- 메인보드의 SW1을 누르면 메인보드 led 초기화 이 SW는 눌렀다가 뗄때에만 동작
- 파란불이 꺼질때까지 남은 시간 7segment로 표시(한방향만)
- 각 거리에 맞는 횡단보도 LED(빨간불/파란불), 꺼지기 2초전 파란불은 깜빡이게 함
- 외부보드에 Skip 스위치
- 기본내용은 GPIO 3개로 구현

## 회로 설계
![image](https://github.com/sunkk8482/FirmwareDiviceDriverProject/assets/86597542/b5881793-51fe-4808-a4c7-392619e001cd)

## 보드
![image](https://github.com/sunkk8482/FirmwareDiviceDriverProject/assets/86597542/fac629de-226b-4190-88fe-7cb176be1d72)
