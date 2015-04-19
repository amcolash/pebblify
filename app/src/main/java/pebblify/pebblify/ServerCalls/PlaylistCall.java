package pebblify.pebblify.ServerCalls;

import android.app.ProgressDialog;
import android.os.AsyncTask;
import android.widget.Toast;

import org.json.JSONObject;

import pebblify.pebblify.AppManager;

/**
 * Created by andrew on 4/19/15.
 */
public class PlaylistCall extends AsyncTask<String, String, JSONObject> {
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
  protected JSONObject doInBackground(String... params) {
    try {
      JSONObject p = Json.getJsonObject(
        "https://api.spotify.com/" + appManager.getId() + "/playlists",
        new String[]{"Authorization"},
        new String[]{"Bearer " + params[0]}
      );
      return p;
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }

  @Override
  protected void onPostExecute(JSONObject json) {
    progress.dismiss();
    try {
      appManager.setId(json.getString("id"));

    } catch (Exception e) {
      e.printStackTrace();
      Toast.makeText(appManager.getContext(), "Something went wrong!", Toast.LENGTH_LONG).show();
    }
  }
}
