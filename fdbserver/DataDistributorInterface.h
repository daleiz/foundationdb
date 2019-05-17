/*
 * DataDistributorInterface.h
 *
 * This source file is part of the FoundationDB open source project
 *
 * Copyright 2013-2019 Apple Inc. and the FoundationDB project authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FDBSERVER_DATADISTRIBUTORINTERFACE_H
#define FDBSERVER_DATADISTRIBUTORINTERFACE_H

#include "fdbrpc/fdbrpc.h"
#include "fdbrpc/Locality.h"
#include "fdbclient/FDBTypes.h"

struct DataDistributorInterface {
	constexpr static FileIdentifier file_identifier = 12383874;
	RequestStream<ReplyPromise<Void>> waitFailure;
	RequestStream<struct HaltDataDistributorRequest> haltDataDistributor;
	struct LocalityData locality;
	RequestStream<struct GetDataDistributorMetricsRequest> dataDistributorMetrics;

	DataDistributorInterface() {}
	explicit DataDistributorInterface(const struct LocalityData& l) : locality(l) {}

	void initEndpoints() {}
	UID id() const { return waitFailure.getEndpoint().token; }
	NetworkAddress address() const { return waitFailure.getEndpoint().getPrimaryAddress(); }
	bool operator== (const DataDistributorInterface& r) const {
		return id() == r.id();
	}
	bool operator!= (const DataDistributorInterface& r) const {
		return !(*this == r);
	}

	template <class Archive>
	void serialize(Archive& ar) {
		serializer(ar, waitFailure, haltDataDistributor, locality, dataDistributorMetrics);
	}
};

struct HaltDataDistributorRequest {
	constexpr static FileIdentifier file_identifier = 1904127;
	UID requesterID;
	ReplyPromise<Void> reply;

	HaltDataDistributorRequest() {}
	explicit HaltDataDistributorRequest(UID uid) : requesterID(uid) {}

	template<class Ar>
	void serialize(Ar& ar) {
		serializer(ar, requesterID, reply);
	}
};

struct GetDataDistributorMetricsReply {
	constexpr static FileIdentifier file_identifier = 1284337;
	Standalone<RangeResultRef> storageMetricsList;

	GetDataDistributorMetricsReply() {}

	template <class Ar>
	void serialize(Ar& ar) {
		serializer(ar,storageMetricsList);
	}
};

struct GetDataDistributorMetricsRequest {
	constexpr static FileIdentifier file_identifier = 1059267;
	KeyRange keys;
	int shardLimit;
	ReplyPromise<struct GetDataDistributorMetricsReply> reply;

	GetDataDistributorMetricsRequest() {}
	explicit GetDataDistributorMetricsRequest(KeyRange const& keys, const int shardLimit) : keys(keys), shardLimit(shardLimit) {}

	template<class Ar>
	void serialize(Ar& ar) {
		serializer(ar, keys, shardLimit, reply);
	}
};

#endif //FDBSERVER_DATADISTRIBUTORINTERFACE_H
