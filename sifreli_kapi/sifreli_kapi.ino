/*
 * Ebubekir Arkalı tarafından yazılmıştır.
 * Soru,öneri ve görüş için ebubekir07a@gmail.com
 * ebubekirarkali.blogspot.com
 * https://www.youtube.com/channel/UCfRC3PPhSJDlqjU_me437KA?view_as=subscriber
 * Instagram: bekrarkali
 */
//Kütüphaneler
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <Keypad.h>
RFID rfid(10,15);//rfid pinleri belirtildi
LiquidCrystal_I2C lcd(0x27,16,2);//display ekranın kaça kaç olduğu belirtildi

//display ekranda tüm pixelleri dolduran özel karakter
byte hepsi[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

String baslangic="Hosgeldiniz";//başlangıç metni atandı.
String sifre="1234";//şifre değişkei atandı
int sure=270;//motorun hareket edeceği süreyi 17 ye böl

//rfid kart kodları
byte kart[5] = {7,14,122,137,250}; 
byte kart2[5] = {136,4,131,136,135};


//pinler
int kapiac = 2;//motorun sağ tarafa dönüşü
int kapikapat=A0;//motorun soö tarafa dönüşü
int red=A2;
int green=A3;
int blue=99;
int buzzer= 9;

//sabit değişkenler
int basildi=0 ;
int kontrol=0;
int sonlandir=0;
int kapidurum=0;
String girilensifre="";//şifrenin boş hali
boolean izin = true;//rfid okuyucu için değişken


//keyped
const byte satir = 4;
const byte sutun = 4;
char tus;
char tus_takimi[satir][sutun] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte satir_pinleri[satir] = {8, 7, 6, 5};
byte sutun_pinleri[satir] = {4, A1, 3, };
Keypad pres = Keypad(makeKeymap(tus_takimi) , satir_pinleri , sutun_pinleri , satir , sutun);

void setup() {
  //çıkış pinleri
  pinMode(kapiac, OUTPUT);
  pinMode(kapikapat, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  
  lcd.begin();//i2c başlatıldı
  Serial.begin(9600);//seri haberleşme başlatıldı

  SPI.begin();//spi haberleşme başlatıldı
  rfid.init();//rfid başlatıldı
  hosgeldiniz();//hoşgeldiniz fonksiyon çağrıldı(ekrana hoşgeldiziniz yazar)

}
void loop(){
  izin=true;//izin adlı değişken true olarak tanımlandı
  tus = pres.getKey();//keypedden tuş bilgisi alınıyor
  
  //ortak anot için mavi yakıldı
  digitalWrite(blue,LOW);
  digitalWrite(red,HIGH);
  digitalWrite(green,HIGH);
  
if (tus != NO_KEY)//eğer keypedde bir tuşa basılmışsa
    {
      //her tuşa basılınca buzzer 200 ms çalışsın
      digitalWrite(buzzer,HIGH);
      delay(200);
      digitalWrite(buzzer,LOW);
      
      switch(tus)//tus değişkenine bilgi geldiğinde switch case e girilsin
      {
      case '#'://eğer tuş kare ise
        if(kapidurum==1){
          lcd.clear(); 
          lcd.print("KAPI KAPANIYOR");
          digitalWrite(kapikapat,HIGH);
          digitalWrite(kapiac,LOW);
          animasyon();
          digitalWrite(kapikapat,LOW);
          digitalWrite(kapiac,LOW);
          hosgeldiniz();
          kontrol=0;
          kapidurum=0;
        }
        break;        
      case '*':eğer tuş yıldız ise
        if(kapidurum==0){
          tuscase();
        }
      break;
      
      default://yıldız veya kare değilse
        tusdefault();
      break;
        }
        }
sifree();//sifree fonksiyonu
rfidd();//rfid fonksiyonu
}
void yildiz(){//şifre yazılırken yıldız işaretinin çıkmasını sağlar
  switch(basildi){
    case 1 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    kare();//şifre alanının tamamını doldurur
    lcd.setCursor(0,1);
    lcd.print("*");

    break;

    case 2 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    kare();
    lcd.setCursor(0,1);
    lcd.print("**");

    break;

    case 3 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    kare();
    lcd.setCursor(0,1);
    lcd.print("***");
 
    break;

    case 4 :
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("  < Sifre >");
    kare();
    lcd.setCursor(0,1);
    lcd.print("****");
    delay(200);
    basildi=0;
    break;
   



  }
}
void hosgeldiniz(){
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(baslangic);
}
void sifree(){//şifre girilmemişken ekranda "Şifre" yazısını çıkartır
    if((kontrol==2)&&(sonlandir==0)){
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("  < Sifre >");
        kare();
        sonlandir=1;
  }
}
void tuscase(){//yıdıza basıldığında çağrılır. iki defa yıldıza basılıp basılmadığını kontrol eder
  kontrol=kontrol+1;
  if(kontrol>=2){
    girilensifre="";
    kontrol=2;
  }
  }
void tusdefault(){//yıldıza basıldıktan sonra şifrenin yazılmasını sağlar
  if(kontrol==2){//iki defa yıldıza basıldığında burası devreye girer
          lcd.clear();
          if(tus){
            basildi++;
          }
          yildiz();//şifre yazma ekranını açar
          girilensifre+=tus;//basılan tuşu girilen şifreye ekler ve şifre oluşur
          if(girilensifre.length()==4){//eğer girilen şifre 4 haneye eşit olursa 
            sonlandir=0;
            if(girilensifre==sifre){ //girilen şifre asıl şifreye eşitse
              digitalWrite(blue,HIGH);
              digitalWrite(red,HIGH);
              digitalWrite(green,LOW);
              lcd.clear(); 
              lcd.print("KAPI ACILIYOR");
              digitalWrite(buzzer,HIGH);
              delay(500);
              digitalWrite(buzzer,LOW);
              
              digitalWrite(kapikapat,LOW);
              digitalWrite(kapiac,HIGH);
              animasyon();
              digitalWrite(kapikapat,LOW);
              digitalWrite(kapiac,LOW);
              kapidurum=1;
              lcd.clear();
            } 
            else if(girilensifre!=sifre){//değilse
              digitalWrite(blue,HIGH);
              digitalWrite(red,LOW);
              digitalWrite(green,HIGH);
              lcd.clear();
              lcd.print("YANLIS SIFRE");
              digitalWrite(buzzer,HIGH);
              delay(500);
              digitalWrite(buzzer,LOW);
              delay(1000);
              lcd.clear();
              }
              kontrol=0;
              basildi=0;
          hosgeldiniz();//işlem bittikten sonra tekrar ekranda hoşgeldiniz yazsın
        
        
        }}
  else{//eğer yıldıza iki defa basmadan işlem yapılmaya çalışılıra
    kontrol=0;
    basildi=0;
    }
}
void rfidd(){
  if(rfid.isCard()){
    digitalWrite(buzzer,HIGH);
    delay(500);
    digitalWrite(buzzer,LOW);
    kontrol=0;
    basildi=0;
    if(rfid.readCardSerial()){
      Serial.print("Kart bulundu ID: ");
      Serial.print(rfid.serNum[0]);
      Serial.print(",");
      Serial.print(rfid.serNum[1]);
      Serial.print(",");
      Serial.print(rfid.serNum[2]);
      Serial.print(",");
      Serial.print(rfid.serNum[3]);
      Serial.print(",");
      Serial.println(rfid.serNum[4]);
    }
    for(int i=0; i<5; i++){
      if((rfid.serNum[i] != kart[i])&&(rfid.serNum[i] != kart2[i])){
        izin = false;
      }
    }
    if(izin==true){
      lcd.clear();
      //Serial.println("izin Verildi");
      
      if(kapidurum==0){
        digitalWrite(green,LOW);
        digitalWrite(red,HIGH);
        digitalWrite(blue,HIGH);
        lcd.print("KAPI ACILIYOR");
        digitalWrite(kapikapat,LOW);
        digitalWrite(kapiac,HIGH);
        animasyon();
        digitalWrite(kapikapat,LOW);
        digitalWrite(kapiac,LOW);
        kapidurum=1;
      }
      else{
        lcd.print("KAPI ZATEN ACIK");
        delay(700);
        lcd.clear();
      }
      hosgeldiniz();
    }
    else{
      lcd.clear();
     //Serial.println("izinsiz Giris");
      digitalWrite(green,HIGH);
      digitalWrite(red,LOW);
      digitalWrite(blue,HIGH);
     lcd.print("IZINSIZ GIRIS");
     delay(1000);
     hosgeldiniz();
    
    }
    rfid.halt();
  }
}
void kare(){//şifre alnını doldurur (4 tane) şifre girildikçe burası silinir yerine yıldız yazılır
  lcd.createChar(1, hepsi);//özel karakter çağrılır
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.setCursor(1,1);
  lcd.write(1);
  lcd.setCursor(2,1);
  lcd.write(1);
  lcd.setCursor(3,1);
  lcd.write(1);

}
void animasyon(){//motor çalışırken oluşan animasyon
  lcd.createChar(1, hepsi);//özel karakter çağrılıp girilen süre ile ekrana sıralanır.
  lcd.setCursor(0,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(1,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(2,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(3,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(4,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(5,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(6,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(7,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(8,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(9,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(10,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(11,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(12,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(13,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(14,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(15,1);
  lcd.write(1);
  delay(sure);
  lcd.setCursor(16,1);
  lcd.write(1);
  delay(sure);
}
