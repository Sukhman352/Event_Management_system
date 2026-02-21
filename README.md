Features
 
1. Event Booking
Book different types of events:
Wedding
Conference
Party
Enter customer details (Name & Contact Number)
Select event date with proper validation (dd-mm-yyyy format)
Enter number of guests
Set food cost per person

2. Venue Selection
Users can choose from multiple venues:
Grand Ballroom
Beach Resort
Conference Center
Garden
Rooftop Terrace
The system also provides venue availability checking.

3. Additional Services
Users can add multiple services to their event:
Decoration
Photography
Bridal Makeup
Music Band
Services can be selected using numbers and added dynamically.
Each service cost is automatically included in the total bill.

4. Automatic Invoice Generation
After booking, the system generates a detailed Event Invoice, including:
Booking ID
Booking Date & Time
Event Date
Customer Details
Event Type
Venue
Number of Guests
Food Cost
Decoration Cost
Selected Additional Services
Total Service Cost
Final Total Cost

Example Output:

------------ EVENT INVOICE ------------

Booking ID: 1001
Booking Date: Sat Feb 21 20:12:36 2026
Event Date: 12-03-2026
Customer: Sukhman
Contact: 7986465185
Event Type: Wedding
Venue: Garden
Guests: 100

Food Cost: Rs.5000.00
Decoration Cost: Rs.50000.00

Additional Services:
- Bridal Makeup (Rs.15000)
- Photography (Rs.15000)
- Decoration (Rs.15000)
- Music Band (Rs.15000)

Total Service Cost: Rs.60000.00
TOTAL COST: Rs.115000.00

Wedding has been successfully booked!

5. Booking Management
View All Bookings
Search Bookings
Check Venue Availability
Store booking data in file (bookings.txt)

Technologies Used
C++
Object-Oriented Programming (OOP)
File Handling
Date Validation Logic
Dynamic Cost Calculation
Menu-Driven Console Interface

Project Structure
Final Project/
│── event_mang.cpp
│── event_mang.exe
│── bookings.txt
