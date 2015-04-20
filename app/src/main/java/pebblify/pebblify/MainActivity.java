package pebblify.pebblify;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.getpebble.android.kit.PebbleKit;
import com.spotify.sdk.android.authentication.AuthenticationClient;
import com.spotify.sdk.android.authentication.AuthenticationRequest;
import com.spotify.sdk.android.authentication.AuthenticationResponse;

import pebblify.pebblify.Watchers.ReceiveHandlerService;

public class MainActivity extends Activity {

  // TODO: Replace with your client ID
  private static final String CLIENT_ID = "647f87a9edb0446c9ba684f355f9dde7";
  // TODO: Replace with your redirect URI
  private static final String REDIRECT_URI = "pebblify://callback";

  // Request code that will be used to verify if the result comes from correct activity
  // Can be any integer
  private static final int REQUEST_CODE = 1337;

  private AppManager appManager = AppManager.getInstance();

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    appManager.setContext(this);

    if (PebbleKit.isWatchConnected(this)){
      PebbleKit.startAppOnPebble(this, appManager.getUUID());
    }

    AuthenticationRequest.Builder builder = new AuthenticationRequest.Builder(CLIENT_ID,
      AuthenticationResponse.Type.TOKEN,
      REDIRECT_URI);
    builder.setScopes(new String[]{"user-read-private"});
    AuthenticationRequest request = builder.build();

    AuthenticationClient.openLoginActivity(this, REQUEST_CODE, request);

    startService(new Intent(this, ReceiveHandlerService.class));

    Button logout = (Button) findViewById(R.id.logout);
    logout.setOnClickListener(new View.OnClickListener() {
      @Override
      public void onClick(View v) {
        AuthenticationClient.logout(getBaseContext());
      }
    });
  }

  @Override
  protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
    super.onActivityResult(requestCode, resultCode, intent);

    // Check if result comes from the correct activity
    if (requestCode == REQUEST_CODE) {
      AuthenticationResponse response = AuthenticationClient.getResponse(resultCode, intent);
      if (response.getType() == AuthenticationResponse.Type.TOKEN) {
        appManager.setAuthToken(response.getAccessToken());
        appManager.UserRequest();
      }
    }
  }

  @Override
  protected void onDestroy() {
    super.onDestroy();
    stopService(new Intent(this, ReceiveHandlerService.class));
  }
}
