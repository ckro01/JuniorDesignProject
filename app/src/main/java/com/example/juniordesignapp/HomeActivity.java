package com.example.juniordesignapp;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class HomeActivity extends AppCompatActivity {
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
        Log.d("page", "BT mgmnt page loaded");
        Toast.makeText(this, "Loading Bluetooth Settings...", Toast.LENGTH_SHORT).show();
    }
    public void settingsPage(View locate) {
        Intent intent = new Intent(this, settingsPage.class);
        startActivity(intent);
        Log.d("page", "settings page loaded");
        Toast.makeText(this, "Loading Settings...", Toast.LENGTH_SHORT).show();

    }
}