/* teste de leitura da porta USB usando Arduino e a linguagem C */

int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(12, INPUT);
  //Serial.println("Digite um número: ");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  buttonState = digitalRead(12);
  if (buttonState == HIGH){
    Serial.write('1');
    //Serial.println('1');
  }
  
  

}
