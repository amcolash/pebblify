package pebblify.pebblify.Models;

/**
 * Created by andrew on 4/19/15.
 */
public class Playlist {
  private String name;
  private String id;

  public Playlist(String name, String id) {
    this.name = name;
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getId() {
    return id;
  }

  public void setId(String id) {
    this.id = id;
  }
}
