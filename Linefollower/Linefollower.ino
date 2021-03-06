#define L_PWM 4
#define L_DIR 10
#define R_PWM 6
#define R_DIR 9
#define PWM_MAX 165

#define R_LINE_SENSOR A0
#define L_LINE_SENSOR A1
#define BUZZER 10
#define LED 13

const int pwm=50;

int blad = 0;
int blad_poprzedni = 0;
int wzmocnienie_kp = 2;
int liczba_czuj_na_linii = 0;
int granica = 800;
int czujniki_waga[6] = { -30, -20, -10, 10, 20, 30};
int czujniki_adc[6];
int czujniki_bin[6];
const int adc_kanal[] = {A0, A1, A2, A3, A4, A5};

void algorytm()
{
  //zerowanie zmiennych!!!
  blad = 0;
  liczba_czuj_na_linii = 0;
  //odczytaj wartości z czujników i zapisz do tablicy czujniki_adc[] oraz zapełnianie tablicy czujniki)bin[]
  for (int i = 0; i <= 5; i++)
  {
    czujniki_adc[i] = analogRead(adc_kanal[i]);
    if (czujniki_adc[i] < 800) czujniki_bin[i] = 0; else czujniki_bin[i] = 1;
  }

  //obliczamy uchyb aktualny (błąd)
  for (int i = 0; i <= 5; i++)
  {
    if (czujniki_bin[i] == 1)
    {
      blad = blad + czujniki_waga[i];
      liczba_czuj_na_linii++;
    }
  }
//czy robot jest na linii?
if(liczba_czuj_na_linii > 0) blad = blad / liczba_czuj_na_linii;
  else  blad = blad_poprzedni;


  //wypisujemy wartości cyfrowe (binarne) na ekran
  Serial.print("\n\r BLAD=");
  Serial.print(blad);

  Serial.print("    CZUJNIKI= ");
  for(int i=0;i<=5;i++) Serial.print(czujniki_bin[i]);

//wysterowanie silnikow
blad = blad * wzmocnienie_kp;
analogWrite(L_PWM,pwm-blad);
analogWrite(R_PWM,pwm+blad);
//zapamietaj blad poprzedni na wypadek wypadniecia
blad_poprzedni = blad;
  delay(250);
}// -------   KONIEC ALGORYTMU LF ------

void setup() {
  //Konfiguracja pinow od mostka H
  pinMode(L_DIR, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_PWM, OUTPUT);
//zmien jesli silniki kreci sie w zla strone
pinMode(L_DIR,HIGH);
pinMode(R_DIR,HIGH);

  //Konfiguracja pozostalych elementow
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, 0); //Wylaczenie buzzera
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0); //Wylaczenie diody

  Serial.begin(9600);
}

void loop() {
  algorytm();

}

void leftMotor(int V) {
  if (V > 0) { //Jesli predkosc jest wieksza od 0 (dodatnia)
    V = map(V, 0, 100, 0, PWM_MAX);
    digitalWrite(L_DIR, 0); //Kierunek: do przodu
    analogWrite(L_PWM, V); //Ustawienie predkosci
  } else {
    V = abs(V); //Funkcja abs() zwroci wartosc V  bez znaku
    V = map(V, 0, 100, 0, PWM_MAX);
    digitalWrite(L_DIR, 1); //Kierunek: do tyłu
    analogWrite(L_PWM, V); //Ustawienie predkosci
  }
}

void rightMotor(int V) {
  if (V > 0) { //Jesli predkosc jest wieksza od 0 (dodatnia)
    V = map(V, 0, 100, 0, PWM_MAX);
    digitalWrite(R_DIR, 0); //Kierunek: do przodu
    analogWrite(R_PWM, V); //Ustawienie predkosci
  } else {
    V = abs(V); //Funkcja abs() zwroci wartosc V  bez znaku
    V = map(V, 0, 100, 0, PWM_MAX);
    digitalWrite(R_DIR, 1); //Kierunek: do tyłu
    analogWrite(R_PWM, V); //Ustawienie predkosci
  }
}

void stopMotors() {
  analogWrite(L_PWM, 0); //Wylaczenie silnika lewego
  analogWrite(R_PWM, 0); //Wylaczenie silnika prawego
}
