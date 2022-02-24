package com.example.juniordesignapp;

import android.Manifest;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import org.altbeacon.beacon.Beacon;
import org.altbeacon.beacon.BeaconManager;
import org.altbeacon.beacon.BeaconParser;
import org.altbeacon.beacon.RangeNotifier;
import org.altbeacon.beacon.Region;

import java.text.DecimalFormat;
import java.util.Collection;


public class locatePage extends AppCompatActivity {
    private static final String TAG = "Beacon Device Locator: "; //tag placeholder
    private static final int PERMISSION_REQUEST_FINE_LOCATION = 1; //permissions
    private static final int PERMISSION_REQUEST_BACKGROUND_LOCATION = 2;
    //variable assignment
    public boolean flag = true, scan = true;
    public int beacCount, beacPower = 0;
    public double beacDist, beacSignal = 0;
    public String beacName = "";
    TextView status;
    private BeaconManager mBeaconManager = null; //beacon manager object
    DecimalFormat df = new DecimalFormat("#.###"); //float trimming

    @Override
    public void onBackPressed() { //back button condition, turns off scanning when user leaves the page in order to prevent battery drain and consistency issues
        Toast.makeText(locatePage.this, "Turning off scanning...", Toast.LENGTH_SHORT).show();
        mBeaconManager.stopRangingBeacons(new Region("myMonitoringUniqueId", null, null, null));
        Intent intent = new Intent(this, HomeActivity.class);
        startActivity(intent);
        finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_locate_page);
        requestPermissions(new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1234);
        status = findViewById(R.id.DeviceLocator);
        Button toggleButton = (Button) findViewById(R.id.scanner);
        mBeaconManager = BeaconManager.getInstanceForApplication(this); //Initialize beacon object
        mBeaconManager.getBeaconParsers().add(new BeaconParser().setBeaconLayout("m:0-3=4c000215,i:4-19,i:20-21,i:22-23,p:24-24")); //set layout
        mBeaconManager.addRangeNotifier(new RangeNotifier() { //add notifier
            @Override
            public void didRangeBeaconsInRegion(Collection<Beacon> beacons, Region region) {
                if (beacons.size() > 0) { //if there is a device
                    //beacDist = beacons.iterator().next().getDistance() / 5; Depreciated
                    beacName = beacons.iterator().next().getBluetoothName(); //get name
                    beacCount = beacons.size(); //get amount
                    beacSignal = beacons.iterator().next().getRunningAverageRssi(); //get average RSSI
                    beacPower = beacons.iterator().next().getTxPower(); //get power level
                    double temp = (beacPower * 1.5 - beacSignal) / (10 * 4); //RSSI -> Distance
                    double beacDistCalc = Math.pow(10.0, temp); //calculate approx distance
                    Log.i(TAG, "Approx Distance: " + beacDist + "\nCount: " + beacCount + "\nName: " + beacName + "\nTx: " + beacPower + "\nRSSI: " + beacSignal + "\nCalc: " + beacDistCalc);
                    if (scan == true && beacCount >= 0 && beacDist <= 20) { //with scanning enabled
                        status.setText("\nDevice Found:"); //update text
                        status.append("\n" + beacName + "\nCurrent Approximate Distance: " + df.format(beacDistCalc) + " m\n"); //append current device distance
                        status.append("Approx Signal Strength (dB): " + df.format(beacSignal)); //display approx strengh
                        try { //sleep method to help with inconsistencies in connection
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    } else { //if no devices found
                        status.setText("\nNo Devices Found!!");
                    }
                }
            }
        });
        toggleButton.setOnClickListener(new View.OnClickListener() { //what the button does
            @Override
            public void onClick(View v) {

                if (flag == true) { //if the flag is true, start scan, update button text, start ranging, update flags
                    Toast.makeText(locatePage.this, "Starting Scan...", Toast.LENGTH_SHORT).show();
                    toggleButton.setText("Turn off scanning...");
                    mBeaconManager.startRangingBeacons(new Region("myMonitoringUniqueId", null, null, null));
                    flag = false;
                    scan = true;
                } else if (flag == false) { //if the flag is false, stop scanning, update button text, stop ranging, update flags
                    Toast.makeText(locatePage.this, "Turning off scanning...", Toast.LENGTH_SHORT).show();
                    toggleButton.setText("Scan Devices");
                    flag = true;
                    mBeaconManager.stopRangingBeacons(new Region("myMonitoringUniqueId", null, null, null));
                    status.setText("");
                    status.append("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                } else { //exception case
                    Toast.makeText(locatePage.this, "Something broke :(", Toast.LENGTH_SHORT).show();
                }
            }
        });

    }
}