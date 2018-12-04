
import processing.net.*; 

Client myClient; 
color myCol;
void setup() {
  myClient = new Client(this, "192.168.0.7", 80);
  myCol=color(10, 255, 180);
  size(500,500);
}

int pow=2;
int[] transCol(color col) {
  int[] t=new int[3];

  t[0]=floor(pow((red(col)/255.f), pow)*4096);
  t[1]=floor(pow((green(col)/255.f), pow)*4096);
  t[2]=floor(pow((blue(col)/255.f), pow)*4096);
  for(int i=0;i<3;i++){
    t[i]=t[i]<4095?t[i]:4095;
  }
  println(t[0],t[1],t[2]);
  return(t);
}

void setCol(color c) {
  int[] arr=transCol(c);
  myClient.write("R "+arr[0]+" "+arr[1]+" "+arr[2]+"\n");
  println("R "+arr[0]+" "+arr[1]+" "+arr[2]+"\n");
}
int num=0;
void draw() {
  num++;
  setCol(myCol);
  colorMode(HSB);
  myCol=color(((num%4096)/4096.f)*255, 100, 200);
  println(red(myCol),green(myCol),blue(myCol));
  delay(10);

  background(myCol);
} 
