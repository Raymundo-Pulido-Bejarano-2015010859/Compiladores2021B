package com.example.prueba1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        long a = 10;
        long b = 12;
        long c = 0;

        c = a + b ;

        long res ;
        while(a>b){
            b = b + 1;
            if(a == b-2){
                break;
            }
        }
        res = b;
    }
}