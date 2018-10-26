#include <catch.hpp>
#include <string>
#include <vector>

#include "opnmidi_midiplay.hpp"
#include "opnmidi_opn2.hpp"
#include "opnmidi_private.hpp"

typedef OPNMIDIplay::OpnChannel Channel;
typedef Channel::Location Location;
typedef Channel::LocationData LocationData;

static size_t iterated_size(const Channel &channel)
{
    const LocationData *head = channel.users_first;
    size_t size = 0;
    while (head) {
        head = head->next;
        ++size;
    }
    return size;
}

bool consistent_size(const Channel &channel)
{
    return channel.users_size == iterated_size(channel);
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: initialization")
{
    Channel channel;
    REQUIRE(channel.users_size == 0);
    REQUIRE(consistent_size(channel));
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: appending")
{
    Channel channel;

    // insert
    LocationData *user1 = channel.users_find_or_create(Location{0, 1, {0}});
    REQUIRE(user1);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 1);
    REQUIRE(consistent_size(channel));
    REQUIRE(channel.users_find(Location{0, 1, {0}}) == user1);

    // try insert with same note number as previous
    user1 = channel.users_find_or_create(Location{0, 1, {0}});
    REQUIRE(user1);
    REQUIRE(channel.users_size == 1);
    REQUIRE(consistent_size(channel));
    REQUIRE(channel.users_find(Location{0, 1, {0}}) == user1);

    // insert
    LocationData *user2 = channel.users_find_or_create(Location{1, 0, {0}});
    REQUIRE(user2);
    REQUIRE(channel.users_size == 2);
    REQUIRE(consistent_size(channel));
    REQUIRE(channel.users_find(Location{1, 0, {0}}) == user2);

    // try insert with same channel number as previous
    user2 = channel.users_find_or_create(Location{1, 0, {0}});
    REQUIRE(user2);
    REQUIRE(channel.users_size == 2);
    REQUIRE(consistent_size(channel));
    REQUIRE(channel.users_find(Location{1, 0, {0}}) == user2);
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: erasing at the front")
{
    Channel channel;

    LocationData *user3 = channel.users_find_or_create(Location{3, 3, {0}});
    LocationData *user2 = channel.users_find_or_create(Location{2, 2, {0}});
    LocationData *user1 = channel.users_find_or_create(Location{1, 1, {0}});
    REQUIRE(user1);
    REQUIRE(user2);
    REQUIRE(user3);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 3);
    REQUIRE(consistent_size(channel));

    // erase at the front
    channel.users_erase(user1);
    REQUIRE(channel.users_first == user2);
    REQUIRE(channel.users_size == 2);
    REQUIRE(consistent_size(channel));
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: erasing at the back")
{
    Channel channel;

    LocationData *user3 = channel.users_find_or_create(Location{3, 3, {0}});
    LocationData *user2 = channel.users_find_or_create(Location{2, 2, {0}});
    LocationData *user1 = channel.users_find_or_create(Location{1, 1, {0}});
    REQUIRE(user1);
    REQUIRE(user2);
    REQUIRE(user3);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 3);
    REQUIRE(consistent_size(channel));

    // erase at the front
    channel.users_erase(user3);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 2);
    REQUIRE(consistent_size(channel));
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: erasing in the middle")
{
    Channel channel;

    LocationData *user3 = channel.users_find_or_create(Location{3, 3, {0}});
    LocationData *user2 = channel.users_find_or_create(Location{2, 2, {0}});
    LocationData *user1 = channel.users_find_or_create(Location{1, 1, {0}});
    REQUIRE(user1);
    REQUIRE(user2);
    REQUIRE(user3);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 3);
    REQUIRE(consistent_size(channel));

    // erase at the front
    channel.users_erase(user2);
    REQUIRE(channel.users_first == user1);
    REQUIRE(channel.users_size == 2);
    REQUIRE(consistent_size(channel));
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: copy constructor")
{
    Channel channel1;

    LocationData *user3 = channel1.users_find_or_create(Location{3, 3, {0}});
    LocationData *user2 = channel1.users_find_or_create(Location{2, 2, {0}});
    LocationData *user1 = channel1.users_find_or_create(Location{1, 1, {0}});
    REQUIRE(user1);
    REQUIRE(user2);
    REQUIRE(user3);
    REQUIRE(channel1.users_first == user1);
    REQUIRE(channel1.users_size == 3);
    REQUIRE(consistent_size(channel1));

    //
    Channel channel2(channel1);
    REQUIRE(channel2.users_size == 3);
    REQUIRE(consistent_size(channel2));

    LocationData *i1 = channel1.users_first;
    LocationData *i2 = channel2.users_first;
    for(size_t i = 0; i < 3; ++i) {
        REQUIRE(i1 != i2);
        REQUIRE(i1->loc == i2->loc);
        i1 = i1->next;
        i2 = i2->next;
    }
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: copy assignment")
{
    Channel channel1;

    LocationData *user3 = channel1.users_find_or_create(Location{3, 3, {0}});
    LocationData *user2 = channel1.users_find_or_create(Location{2, 2, {0}});
    LocationData *user1 = channel1.users_find_or_create(Location{1, 1, {0}});
    REQUIRE(user1);
    REQUIRE(user2);
    REQUIRE(user3);
    REQUIRE(channel1.users_first == user1);
    REQUIRE(channel1.users_size == 3);
    REQUIRE(consistent_size(channel1));

    //
    Channel channel2 = channel1;
    REQUIRE(channel2.users_size == 3);
    REQUIRE(consistent_size(channel2));

    LocationData *i1 = channel1.users_first;
    LocationData *i2 = channel2.users_first;
    for(size_t i = 0; i < 3; ++i) {
        REQUIRE(i1 != i2);
        REQUIRE(i1->loc == i2->loc);
        i1 = i1->next;
        i2 = i2->next;
    }
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: appending full")
{
    Channel channel;
    Location loc = Location{0, 0, {0}};

    // insert users up to capacity
    for(unsigned i = 0; i < Channel::users_max; ++i) {
        LocationData *user = channel.users_find_or_create(loc);
        REQUIRE(user);
        // increment location
        ++loc.note;
        if(loc.note == 128) {
            ++loc.MidCh;
            loc.note = 0;
        }
    }
    REQUIRE(channel.users_size == Channel::users_max);
    REQUIRE(consistent_size(channel));

    // attempt new insertion
    LocationData *user = channel.users_find_or_create(loc);
    REQUIRE(!user);

    // free a slot and retry
    channel.users_erase(channel.users_first);
    user = channel.users_find_or_create(loc);
    REQUIRE(user);
    REQUIRE(channel.users_size == Channel::users_max);
    REQUIRE(consistent_size(channel));
}

TEST_CASE("[OPNMIDIplay::OpnChannel] User list: assigning empty lists")
{
    Channel channel1;
    REQUIRE(channel1.users_size == 0);
    REQUIRE(consistent_size(channel1));

    Channel channel2;
    REQUIRE(channel2.users_size == 0);
    REQUIRE(consistent_size(channel2));

    channel1 = channel2;
    REQUIRE(channel1.users_size == 0);
    REQUIRE(consistent_size(channel1));
}
