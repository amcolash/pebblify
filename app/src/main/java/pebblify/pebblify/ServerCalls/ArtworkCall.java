package pebblify.pebblify.ServerCalls;

import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.Toast;

import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.json.JSONArray;
import org.json.JSONObject;

import java.net.URI;
import java.util.ArrayList;

import pebblify.pebblify.AppManager;
import pebblify.pebblify.Models.Playlist;

/**
 * Created by andrew on 4/19/15.
 */
public class ArtworkCall extends AsyncTask<String, String, String> {
  private AppManager appManager = AppManager.getInstance();

  @Override
  protected String doInBackground(String... params) {
    try {
      JSONObject p = Json.getJsonObject(
        "https://api.spotify.com/v1/tracks/" + appManager.getCurrentTrack().replace("spotify:track:", ""),
        new String[]{"Authorization"},
        new String[]{"Bearer " + appManager.getAuthToken()}
      );
      JSONObject album = p.getJSONObject("album");
      JSONArray images = album.getJSONArray("images");
      JSONObject art = images.getJSONObject(0);
      String url = art.getString("url");
      return url;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  @Override
  protected void onPostExecute(String url) {
    final String uri = "http://madhacks.wespetal.com/img.php?url=" + url;

    Thread t = new Thread(new Runnable() {
      public void run() {
        StringBuilder builder = new StringBuilder();
        HttpClient client = new DefaultHttpClient();
        HttpGet httpGet = new HttpGet(uri);
        try {
          HttpResponse response = client.execute(httpGet);
          Log.d("HTTPGET", "YASSS!!");
        } catch (Exception e) {
          e.printStackTrace();
        }
      }
    });
    t.start();

  }
}