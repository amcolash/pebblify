package pebblify.pebblify;

import android.content.Context;
import android.os.Handler;
import android.util.Log;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import java.util.UUID;

/**
 * Created by andrew on 4/18/15.
 */
public class RecieveHandler {
  public static final int DATA_KEY = 0;
  private AppManager appManager = AppManager.getInstance();
  private static RecieveHandler instance;

  private RecieveHandler(){
    //private because singleton
  }

  public static RecieveHandler getInstance() {
    if(instance == null){
      //create new instance
      instance = new RecieveHandler();
    }
    return instance;
  }

  public void init() {
    final Handler handler = new Handler();
    PebbleKit.registerReceivedDataHandler(appManager.getContext(), new PebbleKit.PebbleDataReceiver(appManager.getUUID()) {

      @Override
      public void receiveData(final Context context, final int transactionId, final PebbleDictionary data) {

        handler.post(new Runnable() {

          @Override
          public void run() {

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
              Log.e("RECIEVE", "Unsure of what to do now... Recieved a non-string!");
            }

          }

        });
        PebbleKit.sendAckToPebble(appManager.getContext(), transactionId);
      }

    });

  }
}
