package com.bytedance.android.bytehook.sample;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;

import com.bytedance.android.bytehook.ByteHook;
import com.memezhibo.android.thread.ThreadHook;

import java.io.BufferedReader;
import java.io.FileReader;

import static java.lang.Math.log;


public class MainActivity extends AppCompatActivity {
    private String TAG = "bytehook_tag";
    private int curType = -1; // -1: unhook; 0: hook-single; 1: hook-partial; 2: hook-all

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        new Handler().postDelayed(()->{
//        },3000);
    }



    private void hookOrUnhook(int newType) {
//        if(newType == curType) {
//            return;
//        }
//
//        if(curType != -1) {
//            NativeHacker.unhook();
//            curType = -1;
//        }
//
//        if(newType != -1) {
//            NativeHacker.hook(newType);
//            curType = newType;
//        }
    }

    public void onRadioButtonClicked(View view) {
        switch(view.getId()) {
            case R.id.radio_hook_single:
                log(1);
                Log.d("memeHook","java log");
                break;
            case R.id.radio_hook_partial:
                hookOrUnhook(1);
                break;
            case R.id.radio_hook_all:
                hookOrUnhook(2);
                break;
            case R.id.radio_unhook:
                hookOrUnhook(-1);
                break;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public void onTestClick(View view) {
        hookOrUnhook(0);
        Thread t= new Thread(new Runnable() {
            @Override
            public void run() {
            while (true){
//                System.out.println("111111");
            }
            }
        });
        t.start();
    }

    public void onGetRecordsClick(View view) {
        String records = ByteHook.getRecords();
        if (records != null) {
            for (String line : records.split("\n")) {
                Log.i(TAG, line);
            }
        }
    }

    public void onDumpRecordsClick(View view) {
        String pathname = getApplicationContext().getFilesDir() + "/bytehook_records.txt";
//        NativeHacker.dumpRecords(pathname);

        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(pathname));
            String line;
            while ((line = br.readLine()) != null) {
                Log.i(TAG, line);
            }
        } catch (Throwable ignored) {
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (Exception ignored) {
                }
            }
        }
    }
}
