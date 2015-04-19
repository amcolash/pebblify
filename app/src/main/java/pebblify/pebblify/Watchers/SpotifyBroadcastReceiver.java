package pebblify.pebblify.Watchers;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import pebblify.pebblify.AppManager;
import pebblify.pebblify.ServerCalls.ArtworkCall;
import pebblify.pebblify.Watchers.ReceiveHandlerService;

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

      if (trackName.length() >= 24) {
        trackName.substring(0, 24);
      }
      if (artistName.length() >= 24) {
        artistName.substring(0, 24);
      }
      if (albumName.length() >= 24) {
        albumName.substring(0, 24);
      }

      int trackLengthInSec = intent.getIntExtra("length", 0);

      if (!trackId.equals(AppManager.getInstance().getCurrentTrack())) {
        Log.d("SONGS", trackId + ", " + artistName + ", " + albumName + ", " + trackName + ", " + trackLengthInSec);
        AppManager.getInstance().sendString(2, artistName + "|" + trackName + "|" + albumName);
        AppManager.getInstance().setCurrentTrack(trackId);
        new ArtworkCall().execute();
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
