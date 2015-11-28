package example.com.abc2016springcommunicationandroid;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.wearable.DataApi;
import com.google.android.gms.wearable.DataEvent;
import com.google.android.gms.wearable.DataEventBuffer;
import com.google.android.gms.wearable.DataMap;
import com.google.android.gms.wearable.Wearable;

import java.util.Set;




public class MainActivity extends AppCompatActivity implements GoogleApiClient.ConnectionCallbacks, GoogleApiClient.OnConnectionFailedListener, DataApi.DataListener  {

    private final static int DEVICES_DIALOG = 1;
    private final static int ERROR_DIALOG = 2;

    private BluetoothTask bluetoothTask = new BluetoothTask(this);

    private ProgressDialog waitDialog;
    private EditText editText1;
    private EditText editText2;
    private String errorMessage = "";

    private GoogleApiClient mGoogleApiClient;
    private TextView resultview;
    private String resultstr;

    // private Handler _handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        resultview = (TextView) findViewById(R.id.resulttxtview);
        mGoogleApiClient = new GoogleApiClient.Builder(this).addConnectionCallbacks(this).addApi(Wearable.API).build();
        resultstr="";

/*          //なぜか、自動同期でよく失敗するので一旦ボタンで対処
        _handler.postDelayed(new Runnable() {//Timer的な
            @Override
            public void run() {

                bluetoothTask.doSend(resultstr);

                _handler.postDelayed(this, 250);
            }
        }, 250);*/

        Button sendBtn = (Button) findViewById(R.id.sendBtn);
        sendBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                bluetoothTask.doSend(resultstr);
            }
        });
    }

    @SuppressWarnings("deprecation")
    @Override
    protected void onResume() {
        super.onResume();

        mGoogleApiClient.connect();
        // Bluetooth初期化
        bluetoothTask.init();
        // ペアリング済みデバイスの一覧を表示してユーザに選ばせる。
        showDialog(DEVICES_DIALOG);
    }

    @Override
    protected void onDestroy() {
        bluetoothTask.doClose();
        super.onDestroy();
    }

    public void doSetResultText(String text) {
        editText2.setText(text);
    }

    protected void restart() {
        Intent intent = this.getIntent();
        this.finish();
        this.startActivity(intent);
    }

    //----------------------------------------------------------------
    // 以下、ダイアログ関連
    @Override
    protected Dialog onCreateDialog(int id) {
        if (id == DEVICES_DIALOG) return createDevicesDialog();
        if (id == ERROR_DIALOG) return createErrorDialog();
        return null;
    }
    @SuppressWarnings("deprecation")
    @Override
    protected void onPrepareDialog(int id, Dialog dialog) {
        if (id == ERROR_DIALOG) {
            ((AlertDialog) dialog).setMessage(errorMessage);
        }
        super.onPrepareDialog(id, dialog);
    }

    public Dialog createDevicesDialog() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("Select device");

        // ペアリング済みデバイスをダイアログのリストに設定する。
        Set<BluetoothDevice> pairedDevices = bluetoothTask.getPairedDevices();
        final BluetoothDevice[] devices = pairedDevices.toArray(new BluetoothDevice[0]);
        String[] items = new String[devices.length];
        for (int i=0;i<devices.length;i++) {
            items[i] = devices[i].getName();
        }

        alertDialogBuilder.setItems(items, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                // 選択されたデバイスを通知する。そのまま接続開始。
                bluetoothTask.doConnect(devices[which]);
            }
        });
        alertDialogBuilder.setCancelable(false);
        return alertDialogBuilder.create();
    }

    @SuppressWarnings("deprecation")
    public void errorDialog(String msg) {
        if (this.isFinishing()) return;
        this.errorMessage = msg;
        this.showDialog(ERROR_DIALOG);
    }
    public Dialog createErrorDialog() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setTitle("Error");
        alertDialogBuilder.setMessage("");
        alertDialogBuilder.setPositiveButton("Exit", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                finish();
            }
        });
        return alertDialogBuilder.create();
    }

    public void showWaitDialog(String msg) {
        if (waitDialog == null) {
            waitDialog = new ProgressDialog(this);
        }
        waitDialog.setMessage(msg);
        waitDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        waitDialog.show();
    }
    public void hideWaitDialog() {
        waitDialog.dismiss();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        // getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        /*if (id == R.id.action_settings) {
            return true;
        }*/

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPause(){
        super.onPause();
        if(mGoogleApiClient != null && mGoogleApiClient.isConnected()){
            mGoogleApiClient.disconnect();
        }
    }

    @Override
    public void onConnected(Bundle bundle){
        Log.d("TAG", "onConnected");
        Wearable.DataApi.addListener(mGoogleApiClient, this);
    }
    @Override
    public void onConnectionSuspended(int i){
        Log.d("TAG", "onConnectionSuspended");
    }
    @Override
    public void onConnectionFailed(ConnectionResult connectionResult){
        Log.e("TAG", "onConnectionFailed");
    }
    @Override
    public void onDataChanged(DataEventBuffer dataEvents) {
        for (DataEvent event : dataEvents) {
            if (event.getType() == DataEvent.TYPE_DELETED) {
                Log.d("TAG", "DataItem deleted: " + event.getDataItem().getUri());
            } else if (event.getType() == DataEvent.TYPE_CHANGED) {
                Log.d("TAG", "DataItem changed: " + event.getDataItem().getUri());
                DataMap dataMap = DataMap.fromByteArray(event.getDataItem().getData());
                float sensordataArray[] = dataMap.getFloatArray("sensordata");
                resultstr = "AccSensorNum:"
                        + "\nX:" + sensordataArray[0]
                        + "\nY:" + sensordataArray[1]
                        + "\nZ:" + sensordataArray[2]
                        + "\n\n" + "GyroSensorNum:"
                        + "\nX:" + sensordataArray[3]
                        + "\nY:" + sensordataArray[4]
                        + "\nZ:" + sensordataArray[5];
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        resultview.setText(resultstr);
                    }
                });
            }
        }
    }
}
