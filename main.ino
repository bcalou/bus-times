#include <LiquidCrystal.h>
#include <TimeLib.h>

// Misc
unsigned long lastMillis;

// Time constants
const int MINUTES_IN_A_DAY = 1440;
const char *DAYS[7] = {"DIM", "LUN", "MAR", "MER", "JEU", "VEN", "SAM"};
const char *MONTHS[12] = {
  "JAN",
  "FEV",
  "MARS",
  "AVR",
  "MAI",
  "JUIN",
  "JUIL",
  "AOUT",
  "SEPT",
  "OCT",
  "NOV",
  "DEC"
};

// Buttons
const int MAIN_BUTTON_PIN = 7;
const int MINUS_BUTTON_PIN = 8;
const int PLUS_BUTTON_PIN = 9;
int mainButtonState = LOW;
int minusButtonState = LOW;
int plusButtonState = LOW;

// Leds
const int GREEN_LED_PIN = 13;
const int YELLOW_LED_PIN = 10;

// LCD Screen
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Settings
bool configScreen = false;
int currentSetting = 0;
const int SETTINGS_POSITIONS[5][2] = {{1, 0}, {7, 0}, {13, 0}, {1, 1}, {6, 1}};
const int SETTINGS_COUNT = 5;

// App constants
const int WALK_TIME = 3;
const int MAX_WAITING_TIME = 120;

// Times
const int BUSES_COUNT = 77;
int BUSES[BUSES_COUNT][3] = {
  {329,	341, 351},
  {360, 371, 381},
  {381, 392, 402},
  {402, 413, 423},
  {413, 424, 434},
  {426, 437, 447},
  {438, 450, 461},
  {449, 462, 474},
  {460, 474, 486},
  {471, 485, 497},
  {482, 496, 508},
  {493, 507, 519},
  {504, 518, 530},
  {515, 529, 541},
  {525, 539, 551},
  {536, 549, 561},
  {547, 560, 572},
  {558, 571, 583},
  {569, 582, 594},
  {580, 593, 605},
  {591, 604, 616},
  {602, 615, 627},
  {613, 626, 638},
  {624, 637, 649},
  {635, 648, 660},
  {646, 659, 671},
  {657, 670, 682},
  {668, 681, 693},
  {679, 692, 704},
  {690, 703, 715},
  {701, 714, 726},
  {712, 725, 737},
  {723, 736, 748},
  {734, 747, 759},
  {745, 758, 770},
  {756, 769, 781},
  {767, 780, 792},
  {778, 791, 803},
  {789, 802, 814},
  {800, 813, 825},
  {811, 824, 836},
  {822, 835, 847},
  {833, 846, 858},
  {844, 857, 869},
  {855, 868, 880},
  {866, 879, 891},
  {877, 890, 902},
  {888, 901, 913},
  {899, 912, 924},
  {910, 923, 935},
  {921, 934, 946},
  {932, 945, 957},
  {943, 956, 968},
  {954, 967, 979},
  {965, 978, 990},
  {976, 990, 1002},
  {987, 1001, 1013},
  {998, 1011,	1023},
  {1009, 1022, 1034},
  {1020, 1032, 1044},
  {1031, 1043, 1055},
  {1042, 1054, 1066},
  {1053, 1065, 1077},
  {1063, 1075, 1087},
  {1073, 1084, 1096},
  {1084, 1095, 1107},
  {1095, 1106, 1117},
  {1106, 1117, 1128},
  {1117, 1128, 1139},
  {1128, 1139, 1149},
  {1140, 1151, 1161},
  {1155, 1166, 1176},
  {1183, 1194, 1204},
  {1204, 1215, 1225},
  {1230, 1241, 1251},
  {1265, 1276, 1286},
  {1303, 1314, 1324}
};

const int SATURDAY_BUSES_COUNT = 53;
int SATURDAY_BUSES[SATURDAY_BUSES_COUNT][3] = {
  {376, 385, 395},
  {405, 414, 424},
  {446, 456, 466},
  {460, 471, 481},
  {476, 487, 497},
  {492, 503, 513},
  {508, 519, 529},
  {524, 535, 545},
  {540, 550, 560},
  {556, 566, 576},
  {572, 582, 592},
  {588, 598, 608},
  {604, 614, 624},
  {620, 630, 640},
  {636, 646, 656},
  {652, 662, 672},
  {668, 678, 688},
  {684, 694, 704},
  {700, 710, 720},
  {716, 726, 736},
  {732, 742, 752},
  {748, 758, 768},
  {764, 774, 784},
  {781, 791, 801},
  {796, 806, 816},
  {812, 822, 832},
  {828, 838, 848},
  {844, 854, 864},
  {860, 870, 880},
  {876, 886, 896},
  {892, 902, 912},
  {908, 918, 929},
  {924, 934, 946},
  {940, 950, 961},
  {956, 967, 978},
  {972, 983, 994},
  {988, 999, 1010},
  {1004, 1015, 1026},
  {1020, 1031, 1042},
  {1036, 1047, 1058},
  {1052, 1063, 1074},
  {1068, 1079, 1090},
  {1084, 1094, 1105},
  {1100, 1110, 1121},
  {1116, 1126, 1137},
  {1131, 1141, 1152},
  {1147, 1157, 1168},
  {1163, 1173, 1184},
  {1190, 1200, 1211},
  {1209, 1219, 1230},
  {1237, 1247, 1258},
  {1268, 1278, 1289},
  {1308, 1318, 1328}
};

const int HOLIDAY_BUSES_COUNT = 13;
int HOLIDAY_BUSES[HOLIDAY_BUSES_COUNT][3] = {
  {480,	491, 495},
  {535, 546, 550},
  {595, 605, 610},
  {655, 665, 670},
  {715, 725, 730},
  {775, 785, 790},
  {835, 845, 850},
  {895, 905, 910},
  {955, 965, 970},
  {1015, 1025, 1030},
  {1075, 1085, 1090},
  {1135, 1145, 1150},
  {1195, 1205, 1210}
};

const int NIGHT_BUSES_COUNT = 4;
int NIGHT_BUSES[NIGHT_BUSES_COUNT][3] = {
  {1355, 1363, 1390},
  {1395, 1403, 1430},
  {1435, 3, 30},
  {1475, 43, 70}
};

// Buses
int* busesList[100];
const int* nextBus = BUSES[0];
const int* followingBus = BUSES[0];
bool serviceEnd = false;

void setup() {
  Serial.begin(9600);
  setTime(12, 6, 0, 11, 3, 2021);
  lcd.begin(16, 2); // 16 columns, 2 rows
  setNextBus();
  update();
  pinMode(MAIN_BUTTON_PIN, INPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
}

// Set bus list based on the current day
void setBusList() {
  emptyBusList();
  int dayBusesCount = addDaysBus();

  // Add night buses if friday or saturday
  if (hasNightBuses()) {
    Serial.println("Has night buses");
    for (int i = 0; i < NIGHT_BUSES_COUNT; i++) {
      busesList[i + dayBusesCount] = NIGHT_BUSES[i];
    }
  } else {
    Serial.println("No night buses");

  }
}

// Add buses for the day and return the number of buses
int addDaysBus() {
  int dayBusesCount;

  if (todayIsHoliday()) { // Holiday or sunday
    dayBusesCount = HOLIDAY_BUSES_COUNT;
    for (int i = 0; i < HOLIDAY_BUSES_COUNT; i++) {
      busesList[i] = HOLIDAY_BUSES[i];
    }
  } else if (isSaturday()) {
    dayBusesCount = SATURDAY_BUSES_COUNT;
    for (int i = 0; i < SATURDAY_BUSES_COUNT; i++) {
      busesList[i] = SATURDAY_BUSES[i];
    }
  } else { // Normal week day
    dayBusesCount = BUSES_COUNT;
    for (int i = 0; i < BUSES_COUNT; i++) {
      busesList[i] = BUSES[i];
    }
  }

  return dayBusesCount;
}

// Fill the bus list with NULL values
void emptyBusList() {
  for (int i = 0; i < 100; i++) {
    busesList[i] = NULL;
  }
}

// Return true if night buses are active
// TODO : false if jour férié
bool hasNightBuses() {
  return
    (weekday() == 6 && hour() > 12 && !todayIsHoliday()) // Friday evening
    || (weekday() == 7 && hour() < 12 && !yesterdayWasHoliday()) // Saturday night
    || (weekday() == 7 && hour() > 12 && !todayIsHoliday()) // Saturday evening
    || (weekday() == 1 && hour() < 12 && !yesterdayWasHoliday()); // Sunday night
}

// Return true if the day is sunday or a holiday
bool todayIsHoliday() {
  return weekday() == 1 || isHoliday();
}

// Return true if yesterday was a holiday
bool yesterdayWasHoliday() {
  adjustTime(-86400);
  bool wasHoliday = weekday() == 2 || isHoliday();
  adjustTime(86400);

  return wasHoliday;
}

// Return true if the day is a holiday
bool isHoliday() {
  return (month() == 1 && day() == 1) // Jour de l'an
    || (month() == 4 && day() == 5) // Lundi de Pâques
    || (month() == 5 && day() == 1) // Fête du travail
    || (month() == 5 && day() == 8) // Victoire 1945
    || (month() == 5 && day() == 13) // Ascension
    || (month() == 5 && day() == 24) // Pentecôte
    || (month() == 7 && day() == 14) // Fête nationale
    || (month() == 8 && day() == 15) // Assomption
    || (month() == 11 && day() == 1) // Toussaint
    || (month() == 11 && day() == 11) // Armistice 1918
    || (month() == 12 && day() == 25); // Noël
}

// Return true if the day is saturday
bool isSaturday() {
  return weekday() == 7;
}

// Find what bus is the next and store it
void setNextBus() {
  setBusList();
  nextBus = NULL;
  followingBus = NULL;

  for (int i = 0; i < 100; i++) {
    if (isNextBus(busesList[i])) {
      nextBus = busesList[i];

      if (busesList[i + 1]) {
        followingBus = busesList[i + 1];
      }

      break;
    }
  }
}

// Return true if the given bus is the next
bool isNextBus(int *bus) {
  return (
    bus[0] > getMinuteOfTheDay() - 1 + WALK_TIME
    && bus[0] - getMinuteOfTheDay() < MAX_WAITING_TIME
  ) || (
    bus[0] % MINUTES_IN_A_DAY > getMinuteOfTheDay() - 1 + WALK_TIME
    && bus[0] % MINUTES_IN_A_DAY - getMinuteOfTheDay() < MAX_WAITING_TIME
  );
}

// Get the minute of the day (from 0 to 1439)
int getMinuteOfTheDay() {
  return (hour() * 60) + minute();
}

// Global update, every minute
void update() {
  setNextBus();

  if (nextBus) {
    printTimes();
  } else {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, LOW);
    lcd.clear();
    printServiceEnd(0);
  }
}

// Get the time between now and the next bus, with the walk time
int getWaitingTime(const int* bus) {
  return (bus[0] - getMinuteOfTheDay() - WALK_TIME) % MINUTES_IN_A_DAY;
}

// Print the end service message on the required row
void printServiceEnd(int row) {
  lcd.setCursor(0, row);
  lcd.print("Fin de service");
}

// Print the times for the two buses
void printTimes() {
  lcd.clear();

  printBusTimes(nextBus, 0);

  if (followingBus) {
    printBusTimes(followingBus, 1);
  } else {
    printServiceEnd(1);
  }
}

// Print the time of the given bus on the given row
void printBusTimes(const int* bus, int row) {
  lcd.setCursor(0, row);

  if (bus) {
    int in = getWaitingTime(bus);

    if (row == 0) {
      digitalWrite(GREEN_LED_PIN, in <= 2 ? HIGH : LOW);
      digitalWrite(YELLOW_LED_PIN, in == 0 ? HIGH : LOW);
    }

    lcd.print(in == 0 ? "GO!" : String(in) + "m");
    lcd.setCursor(5, row);
    lcd.print(format(bus[1]));
    lcd.setCursor(11, row);
    lcd.print(format(bus[2]));
  } else {
    lcd.print("Fin de service");
  }
}

// Convert minutes to HH:mm display (ex: 123 -> 2h03)
String format(int minutes) {
  String minutesRemainder = String(minutes % 60);
  if (minutes % 60 < 10) {
    minutesRemainder = "0" + minutesRemainder;
  }

  return String(minutes / 60) + "h" + minutesRemainder;
}

// Event listener for main button
void onMainButtonPress() {
  if (!configScreen) {
    configScreen = true;
    currentSetting = 0;
    showConfigScreen();
  } else {
    goToNextSetting();
  }
}

// Event listener for minus button
void onMinusButtonPress() {
  if (configScreen) {
    updateCurrentSetting(-1);
  }
}

// Event listener for plus button
void onPlusButtonPress() {
  if (configScreen) {
    updateCurrentSetting(1);
  }
}

// Init the config screen
void showConfigScreen() {
  lcd.clear();
  
  const String values[5] = {
    String(year()),
    String(MONTHS[month() - 1]),
    String(day()),
    String(hour()) + "h",
    String(minute()) + "m (" + DAYS[weekday() - 1] + ")"
  };

  for (int i = 0; i < 5; i++) {
    lcd.setCursor(SETTINGS_POSITIONS[i][0], SETTINGS_POSITIONS[i][1]);
    lcd.print(values[i]);
  }
  
  printCursor();
}

// Go to next setting, close if last one
void goToNextSetting() {
  if (currentSetting < SETTINGS_COUNT - 1) {
    currentSetting++;
    showConfigScreen();
  } else {
    configScreen = false;
    setNextBus();
    update();
  }
}

// Print cursor before the current setting
void printCursor() {
  lcd.setCursor(
    SETTINGS_POSITIONS[currentSetting][0] - 1,
    SETTINGS_POSITIONS[currentSetting][1]
  );
  lcd.print(">");
}

// Increase the value of the current setting
void updateCurrentSetting(int variation) {
  switch(currentSetting) {
    case 0: // Year
      adjustTime(31536000 * variation);
      break;
    case 1: // Month
      adjustTime(2628000 * variation);
      break;
    case 2: // Day
      adjustTime(86400 * variation);
      break;
    case 3: // Hour
      adjustTime(3600 * variation);
      break;
    case 4: // Minute
      adjustTime(60 * variation);
      break;
    default:
      break;
  }
  showConfigScreen();
}

// TODO : fix minute jump
void loop() {
  if (millis() - lastMillis >= 1 * 60 * 1000UL) {
    lastMillis = millis();
    if (!configScreen) {
      update(); 
    }
  }

  listenButton(MAIN_BUTTON_PIN, mainButtonState, &onMainButtonPress);
  listenButton(MINUS_BUTTON_PIN, minusButtonState, &onMinusButtonPress);
  listenButton(PLUS_BUTTON_PIN, plusButtonState, &onPlusButtonPress);
}

// Call the given callback when the given button is pressed
void listenButton(int pin, int &state, void (*callback)()) {
  if (digitalRead(pin) == HIGH) {
    if (state == LOW) {
      state = HIGH;
      callback();
    }
  } else {
    state = LOW;
  }
}
