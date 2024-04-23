int lectura = A0;
int val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(lectura,INPUT); 
  Serial.begin(9600);
  }

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(lectura);
  Serial.println(val);
  delay(1000);
}
