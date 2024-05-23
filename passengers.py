import csv
import random

def main():
    flights = []
    passengers = []

    tot = 0

    with open('flights.csv', 'r') as f:
        reader = csv.reader(f)
        next(reader)        

        for row in reader:
            fid = int(row[0])
            cap = int(row[3])

            cap = round(cap * random.uniform(0.5, 1.0))

            for _ in range(0, cap):
                passengers.append([tot, fid])
                tot += 1

    random.shuffle(passengers)

    count = 0

    for row in passengers:
        row[0] = count
        count += 1

    with open('passengers.csv', 'w') as f:
        writer = csv.writer(f)
        writer.writerow(["PID", "FID"])

        for p in passengers:
            writer.writerow(p)

if __name__ == "__main__":
    main()