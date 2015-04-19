package pebblify.pebblify;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;
import android.view.KeyEvent;
import android.view.inputmethod.BaseInputConnection;

/**
 * Created by andrew on 4/18/15.
 */
public class AppManager {
  private static AppManager instance;
  private String authToken;
  private Context context;

  private AppManager(){
    //private because singleton
  }

  public static AppManager getInstance() {
    if(instance == null){
      //create new instance
      instance = new AppManager();
    }
    return instance;
  }

  public void setContext(Context context) {
    this.context = context;
  }

  public void setAuthToken(String authToken) {
    this.authToken = authToken;
  }

  public String getAuthToken() {
    return authToken;
  }

  /* Spotify app state management */
  public void playPlaylist(String id) {
    Intent intent = new Intent(MediaStore.INTENT_ACTION_MEDIA_PLAY_FROM_SEARCH);
    intent.setData(Uri.parse(
      "spotify:user:125929022:playlist:4rYmmpjtww5Hd3xqltiFFd"));
    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    context.startActivity(intent);
  }

  public void playRadio(String id) {

  }

  public void favorite(String id) {

  }

  public void play() {
//    BaseInputConnection  mInputConnection = new BaseInputConnection(targerView, true);
//    mInputConnection.sendKeyEvent(new KeyEvent(KeyEvent.KEYCODE_MEDIA_PLAY));
  }

  public void pause() {

  }

  public void next() {

  }

  public void previous() {

  }

  public void volUp() {

  }

  public void volDown() {

  }

}
