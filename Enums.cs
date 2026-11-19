namespace Inpost;

// UWAGA: tych enumów nie było na zdjęciach - wartości są zgadnięte.
// Popraw je, gdy zobaczysz prawdziwe definicje.

public enum ParcelSize {
    Small,
    Medium,
    Large
}

public enum DispatchMethod {
    Locker,
    CourierPickup
}

public enum DeliveryMethod {
    Locker,
    Address
}

public enum ShipmentStatus {
    Created,
    Dispatched,
    AtDistributionCenter,
    OutForDelivery,
    InLocker,
    Delivered
}
