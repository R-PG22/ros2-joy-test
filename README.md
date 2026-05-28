joyパッケージを用いたps4コントローラーの値の取得  
コントローラーの値を/dev/ttyACM0に出力する  
jazzy限定  
pub:"ros2 run joy joy_node"  
sub:"ros2 run joy listener"  
で起動できる  
  
送信形式  
LX:LY:RX:RY:00000000000000000|  
  
0埋めされているところは  
L1 L2 L3 R1 R2 R3 PS SH OP up down right left circle cross triangle square  
のbool値
