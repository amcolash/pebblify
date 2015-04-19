package pebblify.pebblify;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * Created by andrew on 4/18/15.
 */
public class SpotifyBroadcastReceiver extends BroadcastReceiver {
  static final class BroadcastTypes {
    static final String SPOTIFY_PACKAGE = "com.spotify.music";
    static final String PLAYBACK_STATE_CHANGED = SPOTIFY_PACKAGE + ".playbackstatechanged";
    static final String QUEUE_CHANGED = SPOTIFY_PACKAGE + ".queuechanged";
    static final String METADATA_CHANGED = SPOTIFY_PACKAGE + ".metadatachanged";
  }

  @Override
  public void onReceive(Context context, Intent intent) {
    // This is sent with all broadcasts, regardless of type. The value is taken from
    // System.currentTimeMillis(), which you can compare to in order to determine how
    // old the event is.
    long timeSentInMs = intent.getLongExtra("timeSent", 0L);

    String action = intent.getAction();

    if (action.equals(BroadcastTypes.METADATA_CHANGED)) {
      String trackId = intent.getStringExtra("id");
      String artistName = intent.getStringExtra("artist");
      String albumName = intent.getStringExtra("album");
      String trackName = intent.getStringExtra("track");
      int trackLengthInSec = intent.getIntExtra("length", 0);

      if (!trackId.equals(AppManager.getInstance().getCurrentTrack())) {
        Log.d("SONGS", trackId + ", " + artistName + ", " + albumName + ", " + trackName + ", " + trackLengthInSec);
        AppManager.getInstance().sendString(2, trackName);
        AppManager.getInstance().sendString(3, artistName);
        AppManager.getInstance().sendString(4, albumName);
        AppManager.getInstance().setCurrentTrack(trackId);
      }

    } else if (action.equals(BroadcastTypes.PLAYBACK_STATE_CHANGED)) {
      boolean playing = intent.getBooleanExtra("playing", false);
      int positionInMs = intent.getIntExtra("playbackPosition", 0);
      // Do something with extracted information
    } else if (action.equals(BroadcastTypes.QUEUE_CHANGED)) {
      // Sent only as a notification, your app may want to respond accordingly.
    }
  }

}
