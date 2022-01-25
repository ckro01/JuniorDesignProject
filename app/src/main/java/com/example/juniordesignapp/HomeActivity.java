package com.example.juniordesignapp;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.view.View;
import android.os.Handler;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.os.Handler;
import android.widget.Toast;

public class HomeActivity extends AppCompatActivity {
    private static int SPLASH_SCREEN_TIME_OUT=2000;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    //basic event handlers created with logging
    public void helpPage(View help) {
        Intent intent = new Intent(this, helpPage.class);
        startActivity(intent);
        Log.d("page","help page loaded...");
        Toast.makeText(this, "Loading About Page...", Toast.LENGTH_SHORT).show();

    }
    public void locatePage(View locate) {
        Intent intent = new Intent(this, locatePage.class);
        startActivity(intent);
        Log.d("page","locate page loaded");
        Toast.makeText(this,"Loading Device Locator...",Toast.LENGTH_SHORT).show();
    }
    public void devicesPage(View locate) {
        Intent intent = new Intent(this, devicePage.class);
        startActivity(intent);
        Log.d("page","locate page loaded");
        Toast.makeText(this,"Loading Device List...",Toast.LENGTH_SHORT).show();
    }
    public void settingsPage(View locate) {
        Intent intent = new Intent(this, settingsPage.class);
        startActivity(intent);
        Log.d("page","locate page loaded");
        Toast.makeText(this,"Loading Settings...",Toast.LENGTH_SHORT).show();

    }
}