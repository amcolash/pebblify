package pebblify.pebblify.Watchers;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import java.util.UUID;

import pebblify.pebblify.AppManager;

/**
 * Created by andrew on 4/18/15.
 */
public class ReceiveHandlerService extends Service {
  private AppManager appManager = AppManager.getInstance();

  private PebbleKit.PebbleDataReceiver mReceiver;
  private PebbleKit.PebbleAckReceiver ackReceiver;
  private PebbleKit.PebbleNackReceiver nackReceiver;

  public static boolean waitingForResponse;

  @Override
  public IBinder onBind(Intent intent) {
    return null;
  }

  @Override
  public void onCreate() {
    Toast.makeText(this, "My Service Started", Toast.LENGTH_LONG).show();

    mReceiver = new PebbleKit.PebbleDataReceiver(appManager.getUUID()) {

      @Override
      public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {
        PebbleKit.sendAckToPebble(appManager.getContext(), transactionId);

        int i = 0;
        while(data.getInteger(i) == null && data.getString(i) == null) {
          i++;
        }

        Log.d("RECIEVE", "Recieved a new key(" + i + "): " + data.getInteger(i));

        if (data.getInteger(i) != null) {
          switch(data.getInteger(i).intValue()) {
            case 0:
              appManager.resumePause();
              break;
            case 1:
              appManager.previous();
              break;
            case 2:
              appManager.next();
              break;
            case 3:
              appManager.volDown();
              break;
            case 4:
              appManager.volUp();
              break;
            default:
              break;
          }
        } else {
          // We got something unexpected, now what?
          Log.e("RECEIVE", "Unsure of what to do now... Received a non-string!");
        }
      }

    };

    ackReceiver = new PebbleKit.PebbleAckReceiver(appManager.getUUID()) {
      @Override
      public void receiveAck(final Context context, final int transactionId) {
        waitingForResponse = false;
      }
    };

    nackReceiver = new PebbleKit.PebbleNackReceiver(appManager.getUUID()) {
      @Override
      public void receiveNack(final Context context, final int transactionId) {
        waitingForResponse = false;
      }
    };

    PebbleKit.registerReceivedDataHandler(appManager.getContext(), mReceiver);
    PebbleKit.registerReceivedAckHandler(appManager.getContext(), ackReceiver);
    PebbleKit.registerReceivedNackHandler(appManager.getContext(), nackReceiver);

  }


  @Override
  public void onDestroy() {
    super.onDestroy();
//    unregisterReceiver(mReceiver);
//    unregisterReceiver(ackReceiver);
//    unregisterReceiver(nackReceiver);
    Toast.makeText(this, "My Service Stopped", Toast.LENGTH_LONG).show();
    Log.d("SERVICE", "onDestroy");
  }
}
