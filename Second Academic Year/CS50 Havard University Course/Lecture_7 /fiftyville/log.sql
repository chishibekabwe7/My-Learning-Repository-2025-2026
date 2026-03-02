-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Crime scene report to understand what happened
SELECT *
  FROM crime_scene_reports
 WHERE year = 2025
   AND month = 7
   AND day = 28
   AND street = 'Humphrey Street';
-- Result: Theft at 10:15am at the Humphrey Street bakery. Three witnesses interviewed.

-- Witness interview transcripts
SELECT *
  FROM interviews
 WHERE year = 2025
   AND month = 7
   AND day = 28
   AND transcript LIKE '%bakery%';
-- Ruth: thief left in a car within 10 minutes of 10:15am -> check bakery security logs
-- Eugene: thief was withdrawing money at ATM on Leggett Street earlier that morning
-- Raymond: thief made a phone call under 1 minute while leaving; planned to take
--           the earliest flight out of Fiftyville the next day (July 29); asked
--           the person on the call to buy the ticket (accomplice)

-- Bakery parking lot exits between 10:15 and 10:25
SELECT *
  FROM bakery_security_logs
 WHERE year = 2025
   AND month = 7
   AND day = 28
   AND hour = 10
   AND minute BETWEEN 15 AND 25
   AND activity = 'exit';
-- 8 license plates: 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

-- ATM withdrawals on Leggett Street on July 28
SELECT *
  FROM atm_transactions
 WHERE year = 2025
   AND month = 7
   AND day = 28
   AND atm_location = 'Leggett Street'
   AND transaction_type = 'withdraw';
-- 8 account numbers made withdrawals

-- Find people who both exited bakery in that window AND withdrew from Leggett St ATM
SELECT pe.*
  FROM people pe
  JOIN bank_accounts ba
    ON pe.id = ba.person_id
 WHERE ba.account_number IN (28500762, 28296815, 76054385, 49610011,
                              16153065, 25506511, 81061156, 26013199)
   AND pe.license_plate IN ('5P2BI95','94KL13X','6P58WS2','4328GD8',
                             'G412CB7','L93JTIZ','322W7JE','0NTHK55');
-- 4 suspects: Iman (396669), Luca (467400), Diana (514354), Bruce (686048)

-- Finding the earliest flight out of Fiftyville on July 29
SELECT f.*, a.city
  FROM flights f
  JOIN airports a
    ON f.destination_airport_id = a.id
 WHERE f.year = 2025
   AND f.month = 7
   AND f.day = 29
   AND f.origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
 ORDER BY f.hour, f.minute
 LIMIT 1;
-- Flight 36 at 8:20am to New York City

-- Check which of our 4 suspects were on flight 36
SELECT pe.name, pa.seat
  FROM passengers pa
  JOIN people pe
    ON pa.passport_number = pe.passport_number
 WHERE pa.flight_id = 36
   AND pe.id IN (396669, 467400, 514354, 686048);
-- Luca and Bruce were on flight 36

-- Check phone calls under 60 seconds made by the suspects on July 28
SELECT *
  FROM phone_calls
 WHERE year = 2025
   AND month = 7
   AND day = 28
   AND duration < 60
   AND caller IN ('(829) 555-5269', '(389) 555-5198', '(770) 555-1861', '(367) 555-5533');
-- Bruce (367) 555-5533 called (375) 555-8161 for 45 seconds
-- Diana also made a call but she wasn't on the flight, so she is not the thief
-- Bruce matches all three clues: bakery exit, ATM withdrawal, short call, flight 36

-- Find the accomplice: who received Bruce's call?
SELECT *
  FROM people
 WHERE phone_number = '(375) 555-8161';
-- Robin is the accomplice who purchased the flight ticket for Bruce
