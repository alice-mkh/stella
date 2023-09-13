#include "stella-highscore.hxx"

#include "StellaHIGHSCORE.hxx"

#define PADDLE_SENSITIVITY 0.25

static StellaCore *core;

struct _StellaCore
{
  HsCore parent_instance;

  StellaHIGHSCORE *stella;
  HsSoftwareContext *context;

  char *save_path;
};

static void stella_atari_2600_core_init (HsAtari2600CoreInterface *iface);

G_DEFINE_FINAL_TYPE_WITH_CODE (StellaCore, stella_core, HS_TYPE_CORE,
                               G_IMPLEMENT_INTERFACE (HS_TYPE_ATARI_2600_CORE, stella_atari_2600_core_init))

static gboolean
stella_core_load_rom (HsCore      *core,
                      const char  *rom_path,
                      const char  *save_path,
                      GError     **error)
{
  StellaCore *self = STELLA_CORE (core);
  g_autofree char *data = NULL;
  gsize length;

  g_set_str (&self->save_path, save_path);

  if (!g_file_get_contents (rom_path, &data, &length, error))
    return FALSE;

  self->stella->setROM (rom_path, data, length);

  if (!self->stella->create (true)) {
    g_set_error (error, HS_CORE_ERROR, HS_CORE_ERROR_INTERNAL, "Failed to create Stella core");
    return FALSE;
  }

  self->context = hs_core_create_software_context (core,
                                                   self->stella->getVideoWidthMax (),
                                                   self->stella->getVideoHeightMax (),
                                                   HS_PIXEL_FORMAT_XRGB8888_REV);

  return TRUE;
}

static void
stella_core_poll_input (HsCore *core, HsInputState *input_state)
{
  StellaCore *self = STELLA_CORE (core);

  Controller::Type left_type = self->stella->getLeftControllerType ();
  Controller::Type right_type = self->stella->getRightControllerType ();

  switch (left_type) {
  case Controller::Type::Joystick:
  case Controller::Type::Genesis:
  case Controller::Type::Joy2BPlus:
  case Controller::Type::BoosterGrip:
    self->stella->setInputEvent (Event::LeftJoystickUp,    input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_UP);
    self->stella->setInputEvent (Event::LeftJoystickDown,  input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_DOWN);
    self->stella->setInputEvent (Event::LeftJoystickLeft,  input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_LEFT);
    self->stella->setInputEvent (Event::LeftJoystickRight, input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_RIGHT);
    self->stella->setInputEvent (Event::LeftJoystickFire,  input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE);

    if (left_type != Controller::Type::Joystick) {
      self->stella->setInputEvent (Event::LeftJoystickFire5, input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE_5);
      if (left_type != Controller::Type::Genesis)
        self->stella->setInputEvent (Event::LeftJoystickFire9, input_state->atari_2600.joystick[0] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE_9);
    }
    break;

  case Controller::Type::Driving:
    self->stella->setInputEvent (Event::LeftDrivingAnalog, (int) round (input_state->atari_2600.driving_axis[0] * 0x7fff));
    self->stella->setInputEvent (Event::LeftDrivingFire,                input_state->atari_2600.driving_fire[0]);
    break;

  case Controller::Type::Paddles:
    self->stella->setInputEvent (Event::LeftPaddleAAnalog, (int) round (input_state->atari_2600.paddle_axis[0] * PADDLE_SENSITIVITY * 0x7fff));
    self->stella->setInputEvent (Event::LeftPaddleAFire,                input_state->atari_2600.paddle_fire[0]);

    self->stella->setInputEvent (Event::LeftPaddleBAnalog, (int) round (input_state->atari_2600.paddle_axis[1] * PADDLE_SENSITIVITY * 0x7fff));
    self->stella->setInputEvent (Event::LeftPaddleBFire,                input_state->atari_2600.paddle_fire[1]);
    break;

  default:
    break;
  }

  switch (right_type) {
  case Controller::Type::Joystick:
    self->stella->setInputEvent (Event::RightJoystickUp,    input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_UP);
    self->stella->setInputEvent (Event::RightJoystickDown,  input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_DOWN);
    self->stella->setInputEvent (Event::RightJoystickLeft,  input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_LEFT);
    self->stella->setInputEvent (Event::RightJoystickRight, input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_RIGHT);
    self->stella->setInputEvent (Event::RightJoystickFire,  input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE);

    if (left_type != Controller::Type::Joystick) {
      self->stella->setInputEvent (Event::RightJoystickFire5, input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE_5);
      if (right_type != Controller::Type::Genesis)
        self->stella->setInputEvent (Event::RightJoystickFire9, input_state->atari_2600.joystick[1] & 1 << HS_ATARI_2600_JOYSTICK_BUTTON_FIRE_9);
    }
    break;

  case Controller::Type::Driving:
    self->stella->setInputEvent (Event::RightDrivingAnalog, (int) round (input_state->atari_2600.driving_axis[1] * 0x7fff));
    self->stella->setInputEvent (Event::RightDrivingFire,                input_state->atari_2600.driving_fire[1]);
    break;

  case Controller::Type::Paddles:
    self->stella->setInputEvent (Event::RightPaddleAAnalog, (int) round (input_state->atari_2600.paddle_axis[2] * PADDLE_SENSITIVITY * 0x7fff));
    self->stella->setInputEvent (Event::RightPaddleAFire,                input_state->atari_2600.paddle_fire[2]);

    self->stella->setInputEvent (Event::RightPaddleBAnalog, (int) round (input_state->atari_2600.paddle_axis[3] * PADDLE_SENSITIVITY * 0x7fff));
    self->stella->setInputEvent (Event::RightPaddleBFire,                input_state->atari_2600.paddle_fire[3]);
    break;

  default:
    break;
  }

  self->stella->setInputEvent (Event::ConsoleColor,      input_state->atari_2600.tv_type == HS_ATARI_2600_TV_TYPE_COLOR);
  self->stella->setInputEvent (Event::ConsoleBlackWhite, input_state->atari_2600.tv_type == HS_ATARI_2600_TV_TYPE_BLACK_WHITE);
  self->stella->setInputEvent (Event::ConsoleLeftDiffA,  input_state->atari_2600.left_difficulty == HS_ATARI_2600_DIFFICULTY_ADVANCED);
  self->stella->setInputEvent (Event::ConsoleLeftDiffB,  input_state->atari_2600.left_difficulty == HS_ATARI_2600_DIFFICULTY_BEGINNER);
  self->stella->setInputEvent (Event::ConsoleRightDiffA, input_state->atari_2600.right_difficulty == HS_ATARI_2600_DIFFICULTY_ADVANCED);
  self->stella->setInputEvent (Event::ConsoleRightDiffB, input_state->atari_2600.right_difficulty == HS_ATARI_2600_DIFFICULTY_BEGINNER);
  self->stella->setInputEvent (Event::ConsoleSelect,     input_state->atari_2600.select_switch);
  self->stella->setInputEvent (Event::ConsoleReset,      input_state->atari_2600.reset_switch);
}

static void
stella_core_run_frame (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  self->stella->runFrame ();

  if (self->stella->getVideoResize ()) {
    HsRectangle area = HS_RECTANGLE_INIT (0, 0, (int) self->stella->getVideoWidth(), (int) self->stella->getVideoHeight());

    hs_software_context_set_area (self->context, &area);
  }

  if(self->stella->getVideoReady ())
    memcpy (hs_software_context_get_framebuffer (self->context),
            self->stella->getVideoBuffer (),
            self->stella->getVideoPitch () * self->stella->getVideoHeight ());

  if (self->stella->getAudioReady ())
    hs_core_play_samples (core, self->stella->getAudioBuffer (), self->stella->getAudioSize () * 2);
}

static void
stella_core_reset (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  self->stella->reset ();
}

static void
stella_core_stop (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  self->stella->destroy ();
  g_clear_object (&self->context);
  g_clear_pointer (&self->save_path, g_free);
}

static void
stella_core_load_state (HsCore          *core,
                        const char      *path,
                        HsStateCallback  callback)
{
  StellaCore *self = STELLA_CORE (core);
  g_autoptr (GFile) file = g_file_new_for_path (path);
  GError *error = NULL;
  g_autofree char *data = NULL;
  size_t size;

  if (!g_file_load_contents (file, NULL, &data, &size, NULL, &error)) {
    callback (core, &error);
    return;
  }

  if (!self->stella->loadState (data, size)) {
    g_set_error (&error, HS_CORE_ERROR, HS_CORE_ERROR_INTERNAL, "Failed to load state");
    callback (core, &error);
    return;
  }

  callback (core, NULL);
}

static void
stella_core_save_state (HsCore          *core,
                        const char      *path,
                        HsStateCallback  callback)
{
  StellaCore *self = STELLA_CORE (core);
  g_autoptr (GFile) file = g_file_new_for_path (path);
  size_t size = self->stella->getStateSize ();
  g_autofree char *data = g_new0 (char, size);
  GError *error = NULL;

  if (!self->stella->saveState (data, size)) {
    g_set_error (&error, HS_CORE_ERROR, HS_CORE_ERROR_INTERNAL, "Failed to save state");
    callback (core, &error);
    return;
  }

  if (!g_file_replace_contents (file, data, size,
                                NULL, FALSE, G_FILE_CREATE_NONE,
                                NULL, NULL, &error)) {
    callback (core, &error);
    return;
  }

  callback (core, NULL);
}

static double
stella_core_get_frame_rate (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  return self->stella->getVideoRate ();
}

static double
stella_core_get_aspect_ratio (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  return self->stella->getVideoAspect ();
}

static double
stella_core_get_sample_rate (HsCore *core)
{
  StellaCore *self = STELLA_CORE (core);

  return self->stella->getAudioRate ();
}

static void
stella_core_finalize (GObject *object)
{
  StellaCore *self = STELLA_CORE (object);

  delete self->stella;

  g_free (self->save_path);

  core = NULL;

  G_OBJECT_CLASS (stella_core_parent_class)->finalize (object);
}

static void
stella_core_class_init (StellaCoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  HsCoreClass *core_class = HS_CORE_CLASS (klass);

  object_class->finalize = stella_core_finalize;

  core_class->load_rom = stella_core_load_rom;
  core_class->poll_input = stella_core_poll_input;
  core_class->run_frame = stella_core_run_frame;
  core_class->reset = stella_core_reset;
  core_class->stop = stella_core_stop;

  core_class->load_state = stella_core_load_state;
  core_class->save_state = stella_core_save_state;

  core_class->get_frame_rate = stella_core_get_frame_rate;
  core_class->get_aspect_ratio = stella_core_get_aspect_ratio;

  core_class->get_sample_rate = stella_core_get_sample_rate;
}

static void
stella_core_init (StellaCore *self)
{
  g_assert (!core);

  core = self;

  self->stella = new StellaHIGHSCORE ();

  self->stella->setAudioStereo (2);
}

static HsAtari2600Controller
get_controller_from_type (Controller::Type type)
{
  switch (type) {
  case Controller::Type::Joystick:
  case Controller::Type::Genesis:
  case Controller::Type::Joy2BPlus:
  case Controller::Type::BoosterGrip:
    return HS_ATARI_2600_CONTROLLER_JOYSTICK;

  case Controller::Type::Driving:
    return HS_ATARI_2600_CONTROLLER_DRIVING;

  case Controller::Type::Paddles:
    return HS_ATARI_2600_CONTROLLER_PADDLES;

  default:
    return HS_ATARI_2600_CONTROLLER_NONE;
  }
}

static HsAtari2600Controller
stella_atari_2600_core_get_left_controller (HsAtari2600Core *core)
{
  StellaCore *self = STELLA_CORE (core);

  return get_controller_from_type (self->stella->getLeftControllerType ());
}

static HsAtari2600Controller
stella_atari_2600_core_get_right_controller (HsAtari2600Core *core)
{
  StellaCore *self = STELLA_CORE (core);

  return get_controller_from_type (self->stella->getRightControllerType ());
}

static void
stella_atari_2600_core_init (HsAtari2600CoreInterface *iface)
{
  iface->get_left_controller = stella_atari_2600_core_get_left_controller;
  iface->get_right_controller = stella_atari_2600_core_get_right_controller;
}

guint32
hs_read_rom (gpointer data)
{
  memcpy (data, core->stella->getROM (), core->stella->getROMSize ());

  return core->stella->getROMSize ();
}

const char *
hs_get_save_path (void)
{
  return core->save_path;
}

GType
hs_get_core_type (void)
{
  return STELLA_TYPE_CORE;
}
