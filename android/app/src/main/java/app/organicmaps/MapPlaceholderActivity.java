package app.organicmaps;

import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import app.organicmaps.display.DisplayChangedListener;
import app.organicmaps.display.DisplayManager;
import app.organicmaps.display.DisplayType;
import app.organicmaps.util.ThemeUtils;

public class MapPlaceholderActivity extends AppCompatActivity implements DisplayChangedListener
{
  @SuppressWarnings("NotNullFieldNotInitialized")
  @NonNull
  private DisplayManager mDisplayManager;
  private boolean mRemoveDisplayListener = true;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setTheme(ThemeUtils.isNightTheme(getApplicationContext()) ? R.style.MwmTheme_Night_FullscreenMenu : R.style.MwmTheme_FullscreenMenu);
    setContentView(R.layout.activity_map_placeholder);
    mDisplayManager = DisplayManager.from(this);
    mDisplayManager.addListener(DisplayType.Device, this);

    findViewById(R.id.btn_continue).setOnClickListener((unused) -> mDisplayManager.changeDisplay(DisplayType.Device));
  }

  @Override
  public void onDisplayChangedToDevice(@NonNull Runnable onTaskFinishedCallback)
  {
    mRemoveDisplayListener = false;
    startActivity(new Intent(this, MwmActivity.class)
        .putExtra(MwmActivity.EXTRA_UPDATE_THEME, true));
    finish();
    onTaskFinishedCallback.run();
  }

  protected void onSafeDestroy()
  {
    if (mRemoveDisplayListener)
      mDisplayManager.removeListener(DisplayType.Device);
  }
}
