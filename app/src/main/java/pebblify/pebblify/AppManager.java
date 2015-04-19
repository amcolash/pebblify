package pebblify.pebblify;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.provider.MediaStore;
import android.view.KeyEvent;

import com.getpebble.android.kit.PebbleKit;
import com.getpebble.android.kit.util.PebbleDictionary;

import java.util.ArrayList;
import java.util.UUID;

import pebblify.pebblify.Models.Playlist;
import pebblify.pebblify.ServerCalls.PlaylistCall;
import pebblify.pebblify.ServerCalls.UserCall;
import pebblify.pebblify.Watchers.ReceiveHandlerService;

/**
 * Created by andrew on 4/18/15.
 */
public class AppManager {
  private static AppManager instance;
  private Context context;
  private final static UUID PEBBLE_APP_UUID = UUID.fromString("1e3227f8-9195-4f77-8c53-fd16c9cba205");

  private String authToken;
  private String id;

  private boolean playing;
  private String currentTrack;
  private ArrayList<Playlist> playlists;

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

  public UUID getUUID() {
    return PEBBLE_APP_UUID;
  }

  public void setContext(Context context) {
    this.context = context;
  }

  public Context getContext() {
    return context;
  }

  public void setAuthToken(String authToken) {
    this.authToken = authToken;
  }

  public String getAuthToken() {
    return authToken;
  }

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }

  public ArrayList<Playlist> getPlaylists() {
    return playlists;
  }

  public void setPlaylists(ArrayList<Playlist> playlists) {
    this.playlists = playlists;
  }

  public void setCurrentTrack(String id) {
    currentTrack = id;
  }

  public String getCurrentTrack() {
    return currentTrack;
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

  public void resumePause() {
    playing = !playing;
    sendKey(KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE);
  }

  public void next() {
    sendKey(KeyEvent.KEYCODE_MEDIA_NEXT);
  }

  public void previous() {
    sendKey(KeyEvent.KEYCODE_MEDIA_PREVIOUS);
  }

  public void volUp() {
    sendKey(KeyEvent.KEYCODE_VOLUME_UP);
  }

  public void volDown() {
    sendKey(KeyEvent.KEYCODE_VOLUME_DOWN);
  }

  private void sendKey(final int keycode) {
    Intent intent = new Intent(Intent.ACTION_MEDIA_BUTTON);
    intent.putExtra(Intent.EXTRA_KEY_EVENT, new KeyEvent(
      KeyEvent.ACTION_UP, keycode));
    context.sendBroadcast(intent);
  }

  public void UserRequest() {
    new UserCall().execute(authToken);
  }

  public static void sendString(int key, String value) {
    while(ReceiveHandlerService.waitingForResponse) {} // Spin until ready
    PebbleDictionary data = new PebbleDictionary();
    data.addString(key, value);
    PebbleKit.sendDataToPebble(AppManager.getInstance().getContext(), AppManager.getInstance().getUUID(), data);
    ReceiveHandlerService.waitingForResponse = true;
  }

}
