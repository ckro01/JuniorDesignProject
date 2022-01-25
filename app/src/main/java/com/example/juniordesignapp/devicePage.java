package com.example.juniordesignapp;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class devicePage extends AppCompatActivity {

    private static final int REQUEST_ENABLE_BT = 0;
    private static final int REQUEST_DISCOVER_BT = 1;


    TextView mStatusBlueTv, mPairedTv;
    ImageView mBlueIv;
    Button mOnBtn,mDiscoverBtn,mPairedBtn;
    BluetoothAdapter mBlueAdapter;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_page);
        //view pairing
        mStatusBlueTv = findViewById(R.id.statusBluetoothTV);
        mPairedTv = findViewById(R.id.pairedTv);
        mBlueIv = findViewById(R.id.bluetoothIv);
        mOnBtn =  findViewById(R.id.onBtn);
        mDiscoverBtn =  findViewById(R.id.discBtn);
        mPairedBtn = findViewById(R.id.pairedBtn);

        //adapter socket
        mBlueAdapter = BluetoothAdapter.getDefaultAdapter();
        //change image to status
        if(mBlueAdapter.isEnabled()) {
            mBlueIv.setImageResource(R.drawable.ic_action_on);
            mStatusBlueTv.setText("Bluetooth is on");
        }
        else {
            mBlueIv.setImageResource(R.drawable.ic_action_off);
            mStatusBlueTv.setText("Bluetooth isn't on");
        }
        //ensure BT is on
        mOnBtn.setOnClickListener(new View.OnClickListener() {
            @Override

            public void onClick(View v) {
                if (!mBlueAdapter.isEnabled()){
                    Toast.makeText(devicePage.this,"Turning on Bluetooth...",Toast.LENGTH_SHORT).show();
                    Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(intent,REQUEST_ENABLE_BT);
                    mBlueIv.setImageResource(R.drawable.ic_action_on);
                    mStatusBlueTv.setText("Bluetooth is on");
                }
                else {

                    Toast.makeText(devicePage.this,"Bluetooth already enabled... Refreshing",Toast.LENGTH_SHORT).show();
                }
                if (mBlueAdapter.isEnabled()) {

                    finish();
                    startActivity(getIntent());}
            }
        });

        mPairedBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

            }
        });

    }
}