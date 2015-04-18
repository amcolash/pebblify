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
}
