package pebblify.pebblify.ServerCalls;

import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;

import pebblify.pebblify.AppManager;
import pebblify.pebblify.Models.Playlist;

/**
 * Created by andrew on 4/19/15.
 */
public class PlaylistCall extends AsyncTask<String, String, JSONArray> {
  public static ProgressDialog progress;
  private AppManager appManager = AppManager.getInstance();

  @Override
  protected void onPreExecute() {
    progress = new ProgressDialog(appManager.getContext());
    progress.setMessage("Getting Data ...");
    progress.setIndeterminate(false);
    progress.setCancelable(true);
    progress.show();

  }

  @Override
  protected JSONArray doInBackground(String... params) {
    try {
      JSONObject p = Json.getJsonObject(
        "https://api.spotify.com/v1/users/" + appManager.getId() + "/playlists",
        new String[]{"Authorization"},
        new String[]{"Bearer " + appManager.getAuthToken()}
      );
      return p.getJSONArray("items");
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  @Override
  protected void onPostExecute(JSONArray json) {
    progress.dismiss();
    try {
      ArrayList<Playlist> playlist = new ArrayList<Playlist>();
      for(int i = 0; i < json.length(); i++) {
        JSONObject temp = json.getJSONObject(i);
        String name = temp.getString("name");
        String id = temp.getString("id");
        playlist.add(new Playlist(name, id));
      }
      appManager.setPlaylists(playlist);

    } catch (Exception e) {
      e.printStackTrace();
      Toast.makeText(appManager.getContext(), "Something went wrong!", Toast.LENGTH_LONG).show();
    }
  }
}
