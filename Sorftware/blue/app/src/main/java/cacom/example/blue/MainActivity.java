package cacom.example.blue;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Calendar;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    TextView Temp,Humi,Light,TimeShow,Set;
    Switch Threshold,Timing;
    Button Set1,Set2,Set3;
    EditText Alarm1,Alarm2,Alarm3;
    Button Connect,Disconnect,Exit;

    AlertDialog.Builder builder;
    AlertDialog alertDialog;

    String TimeString,DateString;

    private LinearLayout Manuallayout;
    private LinearLayout ThreModelayout;
    private LinearLayout Timinglayout;

    private final static int REQUEST_CONNECT_DEVICE = 1;    //宏定义查询设备句柄

    private final static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB";   //SPP服务UUID号

    private InputStream is;    //输入流，用来接收蓝牙数据
    private String smsg = "";    //显示用数据缓存

    BluetoothDevice _device = null;     //蓝牙设备
    BluetoothSocket _socket = null;      //蓝牙通信socket
    boolean bRun = true;
    boolean bThread = false;

    private BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter();    //获取本地蓝牙适配器，即蓝牙设备
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        builder=new AlertDialog.Builder(MainActivity.this).setTitle("Welcome").setMessage("欢迎使用万年历app！！！").setPositiveButton("确定", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                alertDialog.dismiss();
            }
        });
        alertDialog=builder.create();
        alertDialog.show();
        Control_initialization();
        Listen_for_events();
        updateTime();
        /* 解决兼容性问题，6.0以上使用新的API*/
        final int MY_PERMISSION_ACCESS_COARSE_LOCATION = 11;
        final int MY_PERMISSION_ACCESS_FINE_LOCATION = 12;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if(this.checkSelfPermission(android.Manifest.permission.ACCESS_COARSE_LOCATION)!= PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{android.Manifest.permission.ACCESS_COARSE_LOCATION},MY_PERMISSION_ACCESS_COARSE_LOCATION);
                Log.e("11111","ACCESS_COARSE_LOCATION");
            }
            if(this.checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION)!= PackageManager.PERMISSION_GRANTED){
                requestPermissions(new String[]{android.Manifest.permission.ACCESS_FINE_LOCATION},MY_PERMISSION_ACCESS_FINE_LOCATION);
                Log.e("11111","ACCESS_FINE_LOCATION");
            }
        }


        if (_bluetooth == null){
            Toast.makeText(this, "无法打开手机蓝牙，请确认手机是否有蓝牙功能！", Toast.LENGTH_LONG).show();
            finish();
            return;
        }

        // 设置设备可以被搜索
        new Thread(){
            public void run(){
                if(_bluetooth.isEnabled()==false){
                    _bluetooth.enable();
                }
            }
        }.start();


    Connect.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {
         if(_bluetooth.isEnabled()==false){  //如果蓝牙服务不可用则提示
             Toast.makeText(MainActivity.this,"蓝牙连接",1).show();
            _bluetooth.enable();
            return;
        }
          if(_socket==null){
            Intent serverIntent = new Intent(MainActivity.this, DeviceListActivity.class); //跳转程序设置
           startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);  //设置返回宏定义
        }
        }
    });

    Disconnect.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {
                        if(_socket!=null) {
                try {
                    Toast.makeText(MainActivity.this,"蓝牙断开",1).show();

                    bRun = false;
                    //is.close();
                    _socket.close();
                    _socket = null;

                }
                catch (IOException e) {}
            }
        }
    });
    Exit.setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View v) {
             if(_socket!=null){
                //关闭连接socket
                try{
                    bRun = false;
                    //is.close();
                    _socket.close();
                    _socket = null;
                }catch(IOException e){}
            }
            finish();     //退出APP
        }
    });




    }
    //接收活动结果，响应startActivityForResult()
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch(requestCode){
            case REQUEST_CONNECT_DEVICE:     //连接结果，由DeviceListActivity设置返回
                // 响应返回结果
                if (resultCode == Activity.RESULT_OK) {   //连接成功，由DeviceListActivity设置返回
                    // MAC地址，由DeviceListActivity设置返回
                    String address = data.getExtras()
                    .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    // 得到蓝牙设备句柄
                    _device = _bluetooth.getRemoteDevice(address);

                    // 用服务号得到socket
                    try{
                        _socket = _device.createRfcommSocketToServiceRecord(UUID.fromString(MY_UUID));
                    }catch(IOException e){
                        Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                    }
                    //连接socket
                    try{
                        _socket.connect();
                        Toast.makeText(this, "连接"+_device.getName()+"成功！", Toast.LENGTH_SHORT).show();
                    }catch(IOException e){
                        try{
                            Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                            _socket.close();
                            _socket = null;
                        }catch(IOException ee){
                            Toast.makeText(this, "连接失败！", Toast.LENGTH_SHORT).show();
                        }

                        return;
                    }

                    //打开接收线程
                    try{
                        is = _socket.getInputStream();   //得到蓝牙数据输入流
                    }catch(IOException e){
                        Toast.makeText(this, "接收数据失败！", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    if(bThread==false){
                        readThread.start();
                        bThread=true;
                    }else{
                        bRun = true;
                    }
                }
                break;
            default:break;
        }
    }



    //接收数据线程
    Thread readThread=new Thread(){

        public void run(){
            int num = 0;
            byte[] buffer = new byte[1024];
            byte[] buffer_new = new byte[1024];
            int i = 0;
            int n = 0;
            bRun = true;
            //接收线程
            while(true){
                try{
                    while(is.available()==0){
                        while(bRun == false){

                        }
                    }
                    while(true){
                        if(!bThread)//跳出循环
                            return;

                        num = is.read(buffer);         //读入数据
                        n=0;

                        for(i=0;i<num;i++){
                            if((buffer[i] == 0x0d)&&(buffer[i+1]==0x0a)){
                                buffer_new[n] = 0x0a;
                                i++;
                            }else{
                                buffer_new[n] = buffer[i];
                            }
                            n++;
                        }
                        String s = new String(buffer_new,0,n);
                        smsg=s;   //写入接收缓存
                        Log.e("aaaaaa", "onCreate: " + smsg+"\r\n");
                        if(is.available()==0)break;  //短时间没有数据才跳出进行显示
                    }
                    //发送显示消息，进行显示刷新
                 handler.sendMessage(handler.obtainMessage());
                }catch(IOException e){

                }
            }
        }
    };

    //消息处理队列
    @SuppressLint("HandlerLeak")
    Handler handler= new Handler(){
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            int Sensor1 = smsg.indexOf("Sensor1:");
            String temp = smsg;
            if((Sensor1!=-1)&&(temp.indexOf("#",Sensor1))!=-1) {
                Temp.setText(temp.substring(Sensor1+8,temp.indexOf("#",Sensor1))+"℃");   //显示数据
            }else {
//                Fumes.setText("wait");
            }

            int Sensor2 = smsg.indexOf("Sensor2:");
            String humi = smsg;
            if((Sensor2!=-1)&&(humi.indexOf("#",Sensor2))!=-1) {

                Humi.setText(humi.substring(Sensor2+8,humi.indexOf("#",Sensor2))+"%RH");   //显示数据
            }else {

//                Lllumination.setText("wait");
            }

            int Sensor3 = smsg.indexOf("Sensor3:");
            String light = smsg;
            if((Sensor3!=-1)&&(light.indexOf("#",Sensor3))!=-1)  {
                Light.setText(light.substring(Sensor3+8,light.indexOf("#",Sensor3))+"Lux");   //显示数据
            }else {
//                TempInt.setText("wait");
            }

            smsg="";
        }
    };



    private void Control_initialization(){

        Temp=findViewById(R.id.Temp);
        Humi=findViewById(R.id.Humi);
        Light=findViewById(R.id.Light);

        Timinglayout=(LinearLayout)findViewById(R.id.TimeMode);

        Timing=findViewById(R.id.Timing);
        Alarm1=findViewById(R.id.Alarm1);
        Alarm2=findViewById(R.id.Alarm2);
        Alarm3=findViewById(R.id.Alarm3);

        Set=findViewById(R.id.Set);
        Set1=findViewById(R.id.Set1);
        Set2=findViewById(R.id.Set2);
        Set3=findViewById(R.id.Set3);
        TimeShow=findViewById(R.id.TimeShow);

        Timinglayout.setVisibility(View.GONE);

        Connect=findViewById(R.id.Connect);
        Disconnect=findViewById(R.id.Disconnect);
        Exit=findViewById(R.id.Exit);
    }

     private void Listen_for_events(){



         Timing.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
             @Override
             public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                 int n=0;
                 if (isChecked){
                     Timinglayout.setVisibility(View.VISIBLE);
                     if(_socket==null){
                         Toast.makeText(MainActivity.this, "请先连接蓝牙模块", Toast.LENGTH_SHORT).show();
                         return;
                     }else {
                         try {
                             OutputStream os = _socket.getOutputStream();
                             String TxData5="Mode1Open\n";
                             byte[] Mode2 =TxData5.getBytes();//toString(TxData).getBytes();
                             for (int i = 0; i < Mode2.length; i++) {
                                 if (Mode2[i] == 0x0a) n++;
                             }
                             byte[] bos_new5 = new byte[Mode2.length + n];
                             n = 0;
                             for (int i = 0; i < Mode2.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                                 if (Mode2[i] == 0x0a) {
                                     bos_new5[n] = 0x0d;
                                     n++;
                                     bos_new5[n] = 0x0a;
                                 } else {
                                     bos_new5[n] = Mode2[i];
                                 }
                                 n++;
                             }
                             os.write(bos_new5);
                             n=0;
                         } catch (IOException e) {
                             e.printStackTrace();
                         }
                     }
                 }else {
                     Timinglayout.setVisibility(View.GONE);
                     if(_socket==null){
                         Toast.makeText(MainActivity.this, "请先连接蓝牙模块", Toast.LENGTH_SHORT).show();
                         return;
                     }else {
                         try {
                             OutputStream os = _socket.getOutputStream();
                             String TxData6="Mode1Close\n";
                             byte[] Mode2 =TxData6.getBytes();//toString(TxData).getBytes();
                             for (int i = 0; i < Mode2.length; i++) {
                                 if (Mode2[i] == 0x0a) n++;
                             }
                             byte[] bos_new6 = new byte[Mode2.length + n];
                             n = 0;
                             for (int i = 0; i < Mode2.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                                 if (Mode2[i] == 0x0a) {
                                     bos_new6[n] = 0x0d;
                                     n++;
                                     bos_new6[n] = 0x0a;
                                 } else {
                                     bos_new6[n] = Mode2[i];
                                 }
                                 n++;
                             }
                             os.write(bos_new6);
                             n=0;
                         } catch (IOException e) {
                             e.printStackTrace();
                         }
                     }
                 }
             }
         });


         Set1.setOnClickListener(new View.OnClickListener() {
             @Override
             public void onClick(View v) {
                 String alarm1 = Alarm1.getText().toString();
                 int n = 0;
//                 publishmessageplus(mqtt_pub_topic,"Stime"+starttime);
                 try {
                     OutputStream os = _socket.getOutputStream();
                     String TxThre = "Alarm1" + alarm1 + '\n';
                     byte[] Thre = TxThre.getBytes();//toString(TxData).getBytes();
                     for (int i = 0; i < Thre.length; i++) {
                         if (Thre[i] == 0x0a) n++;
                     }
                     byte[] bos_Thre = new byte[Thre.length + n];
                     n = 0;
                     for (int i = 0; i < Thre.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                         if (Thre[i] == 0x0a) {
                             bos_Thre[n] = 0x0d;
                             n++;
                             bos_Thre[n] = 0x0a;
                         } else {
                             bos_Thre[n] = Thre[i];
                         }
                         n++;
                     }
                     os.write(bos_Thre);
                     n = 0;
                 } catch (IOException e) {
                     e.printStackTrace();
                 }
             }
         });

         Set2.setOnClickListener(new View.OnClickListener() {
             @Override
             public void onClick(View v) {
                 String alarm2=Alarm2.getText().toString();
                     int n=0;
//                 publishmessageplus(mqtt_pub_topic,"Etime"+endtime);
                     try {
                         OutputStream os = _socket.getOutputStream();
                         String TxThre="Alarm2"+alarm2+'\n';
                         byte[] Thre =TxThre.getBytes();//toString(TxData).getBytes();
                         for (int i = 0; i < Thre.length; i++) {
                             if (Thre[i] == 0x0a) n++;
                         }
                         byte[] bos_Thre = new byte[Thre.length + n];
                         n = 0;
                         for (int i = 0; i < Thre.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                             if (Thre[i] == 0x0a) {
                                 bos_Thre[n] = 0x0d;
                                 n++;
                                 bos_Thre[n] = 0x0a;
                             } else {
                                 bos_Thre[n] = Thre[i];
                             }
                             n++;
                         }
                         os.write(bos_Thre);
                         n=0;
                     } catch (IOException e) {
                         e.printStackTrace();
                     }
             }
         });

         Set3.setOnClickListener(new View.OnClickListener() {
             @Override
             public void onClick(View v) {
                 String alarm3=Alarm3.getText().toString();
                 int n=0;
//                 publishmessageplus(mqtt_pub_topic,"Etime"+endtime);
                 try {
                     OutputStream os = _socket.getOutputStream();
                     String TxThre="Alarm3"+alarm3+'\n';
                     byte[] Thre =TxThre.getBytes();//toString(TxData).getBytes();
                     for (int i = 0; i < Thre.length; i++) {
                         if (Thre[i] == 0x0a) n++;
                     }
                     byte[] bos_Thre = new byte[Thre.length + n];
                     n = 0;
                     for (int i = 0; i < Thre.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                         if (Thre[i] == 0x0a) {
                             bos_Thre[n] = 0x0d;
                             n++;
                             bos_Thre[n] = 0x0a;
                         } else {
                             bos_Thre[n] = Thre[i];
                         }
                         n++;
                     }
                     os.write(bos_Thre);
                     n=0;
                 } catch (IOException e) {
                     e.printStackTrace();
                 }
             }
         });

         Set.setOnClickListener(new View.OnClickListener() {
             @Override
             public void onClick(View v) {

                 int n=0;
//                 publishmessageplus(mqtt_pub_topic,"Etime"+endtime);
                 try {
                     OutputStream os = _socket.getOutputStream();
                     String TxThre="Ntime"+TimeString+DateString+'\n';
                     byte[] Thre =TxThre.getBytes();//toString(TxData).getBytes();
                     for (int i = 0; i < Thre.length; i++) {
                         if (Thre[i] == 0x0a) n++;
                     }
                     byte[] bos_Thre = new byte[Thre.length + n];
                     n = 0;
                     for (int i = 0; i < Thre.length; i++) { //手机中换行为0a,将其改为0d 0a后再发送
                         if (Thre[i] == 0x0a) {
                             bos_Thre[n] = 0x0d;
                             n++;
                             bos_Thre[n] = 0x0a;
                         } else {
                             bos_Thre[n] = Thre[i];
                         }
                         n++;
                     }
                     os.write(bos_Thre);
                     n=0;
                 } catch (IOException e) {
                     e.printStackTrace();
                 }
             }
         });
     }

    private Handler timeHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            updateTime();
        }
    };
    private void updateTime() {
        String MonthStr,DayStr,HourStr,MinuteStr,SecondStr;

        Calendar calendar = Calendar.getInstance();
        int year = calendar.get(Calendar.YEAR);
        int month = calendar.get(Calendar.MONTH) + 1;
        int day = calendar.get(Calendar.DAY_OF_MONTH);
        int hour = calendar.get(Calendar.HOUR_OF_DAY);
        int minute = calendar.get(Calendar.MINUTE);
        int second = calendar.get(Calendar.SECOND);
        int weekday= calendar.get(Calendar.DAY_OF_WEEK);
        String[] daysOfweek = new String[]{"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
        String dayname=daysOfweek[weekday-1];

        String T =  Integer.toString(hour) +"时" + Integer.toString(minute) + "分"+ Integer.toString(second) + "秒";
        String T1 =  Integer.toString(year) +"年" + Integer.toString(month) + "月"+ Integer.toString(day) + "日";
        MonthStr=Integer.toString(month);
        if (month<10){
            MonthStr="0"+MonthStr;
        }
        DayStr=Integer.toString(day);
        if (day<10){
            DayStr="0"+DayStr;
        }
        HourStr=Integer.toString(hour);
        if (hour<10){
            HourStr="0"+HourStr;
        }
        MinuteStr=Integer.toString(minute);
        if (minute<10){
            MinuteStr="0"+MinuteStr;
        }
        SecondStr=Integer.toString(second);
        if (second<10){
            SecondStr="0"+SecondStr;
        }

        TimeString=HourStr+ MinuteStr+SecondStr;
        DateString=Integer.toString(year)  + MonthStr + DayStr;
        Log.e("aaa",TimeString+DateString);
        TimeShow.setText(T1+"\r\n"+T+"\r\n"+dayname);
        timeHandler.sendEmptyMessageDelayed(0, 1000); // 1000ms = 1s
    }


}