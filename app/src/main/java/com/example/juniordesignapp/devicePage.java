package com.example.juniordesignapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.provider.Settings;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Set;

public class devicePage extends AppCompatActivity {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;


    TextView mStatusBlueTv, mPairedTv;
    ImageView mBlueIv;
    Button mOnBtn, mDiscoverBtn, mPairedBtn;
    BluetoothAdapter mBTAdapter;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_page);
        //variable matching
        mStatusBlueTv = findViewById(R.id.statusBluetoothTV);
        mPairedTv = findViewById(R.id.pairedTv);
        mBlueIv = findViewById(R.id.bluetoothIv);
        mOnBtn =  findViewById(R.id.onBtn);
        mDiscoverBtn =  findViewById(R.id.discBtn);
        mPairedBtn = findViewById(R.id.pairedBtn);

        //adapter socket
        mBTAdapter = BluetoothAdapter.getDefaultAdapter();
        //change image to status
        if (mBTAdapter.isEnabled()) {
            mBlueIv.setImageResource(R.drawable.ic_action_on);
            mStatusBlueTv.setText("Bluetooth is on");
        } else {
            mBlueIv.setImageResource(R.drawable.ic_action_off);
            mStatusBlueTv.setText("Bluetooth isn't on");
        }
        //ensure BT is on
        mOnBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!mBTAdapter.isEnabled()) {
                    Toast.makeText(devicePage.this, "Turning on Bluetooth...", Toast.LENGTH_SHORT).show();
                    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(intent, REQUEST_ENABLE_BT);
                    mBlueIv.setImageResource(R.drawable.ic_action_on);
                    mStatusBlueTv.setText("Bluetooth is on");
                } else {
                    Toast.makeText(devicePage.this, "Bluetooth already enabled... Refreshing", Toast.LENGTH_SHORT).show();
                }
                if (mBTAdapter.isEnabled()) {

                    finish();
                    startActivity(getIntent());
                }
            }
        });
        //Paired devices
        mPairedBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mBTAdapter.isEnabled()) {
                    Toast.makeText(devicePage.this, "Pulling up device list...", Toast.LENGTH_SHORT).show();
                    mPairedTv.setText("Paired List:");
                    Set<BluetoothDevice> devices = mBTAdapter.getBondedDevices();
                    int i = 0;
                    for (BluetoothDevice device : devices) {
                        i++;
                        mPairedTv.append("\n" + i + ": " + device.getName() + " : " + device);
                    }
                }
            }
        });
        mDiscoverBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mBTAdapter.isEnabled() && !mBTAdapter.isDiscovering()) {
                    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                    intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                    startActivityForResult(intent, REQUEST_DISCOVER_BT);
                    Toast.makeText(devicePage.this, "Opening Bluetooth Settings...", Toast.LENGTH_SHORT).show();
                    Toast.makeText(devicePage.this, "Return to the app to make your device discoverable for 5 minutes", Toast.LENGTH_LONG).show();
                    Intent intentOpenBluetoothSettings = new Intent();
                    intentOpenBluetoothSettings.setAction(Settings.ACTION_BLUETOOTH_SETTINGS);
                    startActivity(intentOpenBluetoothSettings);
                }
            }
        });

    }
}