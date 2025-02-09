# Arduino-Safe-Box

O projeto consiste num simples mecanismo de trancar e destrancar uma caixa, mediante a apresentação de password/PIN ou não. 

Materiais utilizados:
- Arduino MEGA;
- Breadboard;
- Sensor ultrassónico;
- Servomotor SG90;
- 2 LEDs;
- 3 Resistências de 220 Ohm;
- 1 Buzzer (passivo ou ativo);
- Display LED 16x2 I2C;
- Matriz de Botões 4x4;
- Sensor de vibração.

Modo de funcionamento:
Com PIN indefinido, a porta estará aberta e o utilizador terá a possibilidade de definir um PIN através da matriz de botões 
ao pressionar o botão A. Nesse caso, a máquina irá passar para um estado onde estará a guardar a password (limitada a 10 caracteres).
Se o utilizador se enganar, pode pressionar C; quando finalizar, deve pressionar #. O utilizador pode trancar a caixa com um botão externo. 

Definido um PIN, o sistema entra em stand-by. O sensor procura indivíduos próximos a cada 100 ms. Em caso de deteção, o display
irá saudar o indivíduo e apresentar as opções B para login e S para reset da password.

Se nas 3 tentativas de log-in o utilizador errar o PIN, o sistema entra em modo LOCK pelo que faz soar um alarme e impede novas tentativas de
log-in ou reset. Em caso de entrada forçada na caixa, um sensor de vibração será ativado o que também leva o sistema o modo LOCK.

Caso o utilizador acerte o PIN, o servmotor roda de modo a destrancar a caixa. O utilizador pode trancar a caixa com um botão externo. 
O sistema volta ao estado de stand-by. 





