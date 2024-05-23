import random
import csv

def main():
    flights = []

    with open('flights.csv', 'r') as f:
        reader = csv.reader(f)
        next(reader)
        
        flights = [row for row in reader]

    num_flights = len(flights)
    num_canceled = 5

    canceled = []

    for _ in range(num_canceled):
        x = random.randint(0, num_flights - 1)
        while x in canceled:
            x = random.randint(0, num_flights - 1)
        
        canceled.append(x)

    canceled = [[canceled[i]] for i in range(num_canceled)]

    with open('canceled.csv', 'w') as f:
        writer = csv.writer(f)
        writer.writerow(['Canceled'])
        writer.writerows(canceled)

if __name__ == "__main__":
    main()