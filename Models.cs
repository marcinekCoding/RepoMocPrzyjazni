namespace Inpost;

public sealed record Sender(
    string Id,
    string Name,
    string PhoneNumber
);

public sealed record Recipient(
    string Id,
    string Name,
    string PhoneNumber
);

public sealed record TrackingEvent(
    DateTime Timestamp,
    ShipmentStatus Status,
    string Location
);

public sealed class Courier {
    public required string Id { get; init; }
    public required string Name { get; init; }
    public required string PhoneNumber { get; init; }
    public required string DeliveryArea { get; init; }
    public int MaxDailyShipments { get; init; } = 30;

    // Tracking codes are stored in assignment order and form the courier's manifest.
    public List<string> AssignedTrackingCodes { get; } = [];
}

public sealed class Parcel {
    public required string Id { get; init; }
    public decimal WeightKg { get; init; }

    // Parcels dropped off at a parcel locker have a declared size.
    // For parcels picked up by a courier from an address, dimensions are provided.
    public ParcelSize? DeclaredSize { get; init; }
    public decimal? LengthCm { get; init; }
    public decimal? WidthCm { get; init; }
    public decimal? HeightCm { get; init; }
}

/// <summary>
/// Represents a parcel locker, i.e., a location in the model
/// that stores shipments and has a limited compartment capacity.
/// </summary>
public sealed class Locker {
    public required string Id { get; init; }
    public required string Name { get; init; }
    public required string Address { get; init; }
    public required string City { get; init; }

    // Locker capacity is limited separately for each parcel size.
    public Dictionary<ParcelSize, int> CapacityBySize { get; init; } = new() {
        [ParcelSize.Small] = 10,
        [ParcelSize.Medium] = 6,
        [ParcelSize.Large] = 3
    };

    // The set prevents the same shipment from occupying a compartment multiple times.
    public Dictionary<ParcelSize, HashSet<string>> StoredShipmentsBySize { get; } =
        Enum.GetValues<ParcelSize>().ToDictionary(size => size, _ => new HashSet<string>(StringComparer.Ordinal));

    /// <summary>Returns the number of shipments occupying compartments of the given size.</summary>
    /// <param name="size">Compartment size.</param>
    /// <returns>Number of tracking codes in the corresponding set of <see cref="StoredShipmentsBySize"/>.</returns>
    /// <exception cref="KeyNotFoundException">Thrown when the dictionary does not contain the key <paramref name="size"/>.</exception>
    public int Occupied(ParcelSize size) => StoredShipmentsBySize[size].Count;

    /// <summary>Calculates the number of available compartments of the given size.</summary>
    /// <param name="size">Compartment size.</param>
    /// <returns>Difference between the capacity and the number of occupied compartments of the given size.</returns>
    public int Available(ParcelSize size) => CapacityBySize[size] - Occupied(size);

    /// <summary>
    /// Attempts to add a shipment tracking code to the set of occupied compartments of the specified size.
    /// Modifies the set on success; does not add the code if capacity is already
    /// exhausted or the code is already in the set.
    /// </summary>
    /// <param name="trackingCode">Shipment tracking code.</param>
    /// <param name="size">Parcel size determining the target set.</param>
    /// <returns><see langword="true"/> if the code was added; <see langword="false"/> if no compartment was available or the code was already in the set.</returns>
    /// <exception cref="KeyNotFoundException">Thrown when <see cref="StoredShipmentsBySize"/> does not contain the key <paramref name="size"/>.</exception>
    public bool TryStore(string trackingCode, ParcelSize size) {
        var stored = StoredShipmentsBySize[size];
        if (stored.Count >= CapacityBySize[size]) {
            return false;
        }
        return stored.Add(trackingCode);
    }

    /// <summary>Removes a shipment tracking code from the set corresponding to the specified size.</summary>
    /// <param name="trackingCode">Tracking code of the shipment to remove.</param>
    /// <param name="size">Parcel size.</param>
    /// <returns><see langword="true"/> if the code was in the set and was removed; otherwise, <see langword="false"/>.</returns>
    /// <exception cref="KeyNotFoundException">Thrown when <see cref="StoredShipmentsBySize"/> does not contain the key <paramref name="size"/>.</exception>
    public bool Remove(string trackingCode, ParcelSize size) => StoredShipmentsBySize[size].Remove(trackingCode);
}

public sealed class ShipmentOrder {
    public required string TrackingCode { get; init; }
    public required string SenderId { get; init; }
    public required string RecipientId { get; init; }
    public required Parcel Parcel { get; init; }

    public DispatchMethod DispatchMethod { get; init; }
    // For locker dispatch, set SourceLockerId; for courier pickup, set SourceAddress.
    public string? SourceLockerId { get; init; }
    public string? SourceAddress { get; init; }

    public DeliveryMethod DeliveryMethod { get; init; }
    // For locker delivery, set DestinationLockerId; for address delivery, set DestinationAddress.
    public string? DestinationLockerId { get; init; }
    public string? DestinationAddress { get; init; }
    public required string DestinationCity { get; init; }

    public DateTime CreatedAt { get; init; }
    public ShipmentStatus Status { get; set; }

    public string? DeliveryCourierId { get; set; }
    public DateOnly? PlannedDeliveryDate { get; set; }

    public DateTime? PickupDeadline { get; set; }
    public DateTime? DeliveredAt { get; set; }
}
