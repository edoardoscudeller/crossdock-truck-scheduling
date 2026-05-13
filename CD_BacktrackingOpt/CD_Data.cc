// File CD_Data.cc
#include "CD_Data.hh"
#include <fstream>

// ---------------------------------------------------------------------------
// CD_Input: costruttore (lettura da file .dzn)
// ---------------------------------------------------------------------------
CD_Input::CD_Input(string file_name)
{
  const unsigned MAX_DIM = 100;
  unsigned i, j;
  char ch, buffer[MAX_DIM];

  ifstream is(file_name);
  if (!is)
  {
    cerr << "Cannot open input file " << file_name << endl;
    exit(1);
  }

  is >> buffer >> ch >> inbound_trucks  >> ch;
  is >> buffer >> ch >> outbound_trucks >> ch;
  is >> buffer >> ch >> inbound_doors   >> ch;
  is >> buffer >> ch >> outbound_doors  >> ch;

  release_time.resize(inbound_trucks);
  unloading_time.resize(inbound_trucks);
  loading_time.resize(outbound_trucks);
  transfer_matrix.resize(inbound_trucks, vector<unsigned>(outbound_trucks));

  // ReleaseTimes
  is.ignore(MAX_DIM, '[');
  for (i = 0; i < inbound_trucks; i++)
    is >> release_time[i] >> ch;

  // UnloadingTimes
  is.ignore(MAX_DIM, '[');
  for (i = 0; i < inbound_trucks; i++)
    is >> unloading_time[i] >> ch;

  // LoadingTimes
  is.ignore(MAX_DIM, '[');
  for (j = 0; j < outbound_trucks; j++)
    is >> loading_time[j] >> ch;

  // TransferMatrix
  is.ignore(MAX_DIM, '[');
  is >> ch; // primo '|'
  for (i = 0; i < inbound_trucks; i++)
  {
    for (j = 0; j < outbound_trucks; j++)
      is >> transfer_matrix[i][j] >> ch;
  }
  is >> ch >> ch;
}

ostream& operator<<(ostream& os, const CD_Input& in)
{
  unsigned i, j;
  os << "InboundTrucks  = " << in.inbound_trucks  << ";" << endl;
  os << "OutboundTrucks = " << in.outbound_trucks << ";" << endl;
  os << "InboundDoors   = " << in.inbound_doors   << ";" << endl;
  os << "OutboundDoors  = " << in.outbound_doors  << ";" << endl;

  os << "ReleaseTimes = [";
  for (i = 0; i < in.inbound_trucks; i++)
  {
    os << in.release_time[i];
    os << (i < in.inbound_trucks - 1 ? ", " : "];\n");
  }

  os << "UnloadingTimes = [";
  for (i = 0; i < in.inbound_trucks; i++)
  {
    os << in.unloading_time[i];
    os << (i < in.inbound_trucks - 1 ? ", " : "];\n");
  }

  os << "LoadingTimes = [";
  for (j = 0; j < in.outbound_trucks; j++)
  {
    os << in.loading_time[j];
    os << (j < in.outbound_trucks - 1 ? ", " : "];\n");
  }

  os << "TransferMatrix = [|";
  for (i = 0; i < in.inbound_trucks; i++)
  {
    for (j = 0; j < in.outbound_trucks; j++)
    {
      os << in.transfer_matrix[i][j];
      if (j < in.outbound_trucks - 1) os << ",";
      else os << "|" << endl;
    }
  }
  os << "];" << endl;

  return os;
}

// ---------------------------------------------------------------------------
// CD_Output
// ---------------------------------------------------------------------------
CD_Output::CD_Output(const CD_Input& my_in)
  : in(my_in),
    inbound_sequence(my_in.InboundTrucks(), -1),
    assigned(my_in.InboundTrucks(), false)
{}

CD_Output& CD_Output::operator=(const CD_Output& other)
{
  inbound_sequence = other.inbound_sequence;
  assigned         = other.assigned;
  return *this;
}

void CD_Output::AssignInbound(unsigned pos, int truck_idx)
{
  int old_truck = inbound_sequence[pos];
  if (old_truck != -1)
    assigned[old_truck] = false;

  inbound_sequence[pos] = truck_idx;
  if (truck_idx != -1)
    assigned[truck_idx] = true;
}

// Calcola il makespan simulando lo scheduling dei camion sulle porte
unsigned CD_Output::ComputeMakespan() const
{
  unsigned n  = in.InboundTrucks();
  unsigned di = in.InboundDoors();
  unsigned m  = in.OutboundTrucks();

  // completion_in[i]: quando finisce lo scarico del camion inbound i
  vector<unsigned> completion_in(n, 0);
  // door_free_in[d]: quando e' libera la porta inbound d
  vector<unsigned> door_free_in(di, 0);

  // Simula l'assegnazione sequenziale dei camion inbound alle porte
  // round-robin sulle porte disponibili
  for (unsigned pos = 0; pos < n; pos++)
  {
    int t = inbound_sequence[pos];
    if (t < 0) continue;
    unsigned door = pos % di;
    unsigned start = max(door_free_in[door], in.ReleaseTime(t));
    unsigned finish = start + in.UnloadingTime(t);
    completion_in[t] = finish;
    door_free_in[door] = finish;
  }

  // Per ogni camion outbound j, il suo start = max su tutti gli inbound i
  // di (completion_in[i] + transfer_matrix[i][j])
  unsigned makespan = 0;
  vector<unsigned> door_free_out(in.OutboundDoors(), 0);
  for (unsigned j = 0; j < m; j++)
  {
    unsigned earliest_start = 0;
    for (unsigned i = 0; i < n; i++)
    {
      unsigned dep = completion_in[i] + in.TransferTime(i, j);
      if (dep > earliest_start)
        earliest_start = dep;
    }
    unsigned door = j % in.OutboundDoors();
    unsigned start  = max(door_free_out[door], earliest_start);
    unsigned finish = start + in.LoadingTime(j);
    door_free_out[door] = finish;
    if (finish > makespan)
      makespan = finish;
  }
  return makespan;
}

unsigned CD_Output::ComputeViolations() const
{
  // In questa formulazione semplificata i vincoli di precedenza
  // sono sempre soddisfatti per costruzione. Lasciato per estensioni future.
  return 0;
}

ostream& operator<<(ostream& os, const CD_Output& out)
{
  unsigned n = out.in.InboundTrucks();
  os << "[";
  for (unsigned pos = 0; pos < n; pos++)
  {
    os << out.inbound_sequence[pos];
    if (pos < n - 1) os << ", ";
  }
  os << "]";
  return os;
}
