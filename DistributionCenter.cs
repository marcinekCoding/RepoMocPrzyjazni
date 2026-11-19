namespace Inpost;

public sealed class DistributionCenter {
    public required string Id { get; init; }
    public required string Name { get; init; }
    public required string City { get; init; }

    public Dictionary<string, ShipmentOrder> Shipments { get; } = new(StringComparer.Ordinal);
    public Dictionary<string, Sender> Senders { get; } = new(StringComparer.Ordinal);
    public Dictionary<string, Recipient> Recipients { get; } = new(StringComparer.Ordinal);
    public Dictionary<string, Courier> Couriers { get; } = new(StringComparer.Ordinal);
    public Dictionary<string, Locker> Lockers { get; } = new(StringComparer.Ordinal);

    public Dictionary<string, List<TrackingEvent>> TrackingHistory { get; } = new(StringComparer.Ordinal);

    // Shipments that have arrived at the center and are awaiting assignment of a delivery courier.
    public Queue<string> ShipmentsAwaitingAssignment { get; } = new();

    /// <summary>Retrieves a shipment by its tracking code.</summary>
    public ShipmentOrder GetShipmentOrThrow(string trackingCode) =>
        Shipments.TryGetValue(trackingCode, out var shipment) ? shipment :
        throw new KeyNotFoundException($"Nie znaleziono przesyłki '{trackingCode}'.");

    /// <summary>Retrieves a courier by their identifier.</summary>
    public Courier GetCourierOrThrow(string courierId) =>
        Couriers.TryGetValue(courierId, out var courier) ? courier :
        throw new KeyNotFoundException($"Nie znaleziono kuriera '{courierId}'.");
}
