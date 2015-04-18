package pebblify.pebblify;

/**
 * Created by andrew on 4/18/15.
 */
public class AppManager {
  private static AppManager instance;
  private String authToken;

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

  public void setAuthToken(String authToken) {
    this.authToken = authToken;
  }

  public String getAuthToken() {
    return authToken;
  }

  /* Spotify app state management */
  public void playPlaylist(String id) {

  }

  public void playRadio(String id) {

  }

  public void favorite(String id) {

  }

  public void play() {

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
